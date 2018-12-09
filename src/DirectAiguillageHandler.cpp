#include "DirectAiguillageHandler.h"
#include "AiguillageManager.h"

template <typename T, typename D>
bool isContainedInVector(const std::vector<T> &toAnalyze, const D& item)
{
    for (int loop = 0; loop != toAnalyze.size(); loop++)
    {
        if (toAnalyze[loop] == item)
            return true;
    }
    return false;
}

int DirectAiguillageHandler::i_nextAvailableId = 0;
int DirectAiguillageHandler::i_nextAvailableAiguillageId = 0;

const std::vector <BaseAiguillage::AiguillageType> DirectAiguillageHandler::compatiblesAiguillages = std::vector<BaseAiguillage::AiguillageType>({BaseAiguillage::AiguillageType::SimpleAiguillage1, BaseAiguillage::AiguillageType::DoubleAiguillage2});

DirectAiguillageHandler::DirectAiguillageHandler(AiguillageManager* parent, std::string name) : SinglePinModeCompatibleAiguillageHandler(parent)
{
    m_parent = parent;
    m_nom = name;
    registerJSonNode();


}

void DirectAiguillageHandler::backup(nlohmann::json saveData)
{
    SinglePinModeCompatibleAiguillageHandler::backup(saveData);
    nlohmann::json actualData = saveData["SinglePinModeCompatibleAiguillageHandler"]["DirectAiguillageHandler"];
    claimedPins.insert(claimedPins.end(), actualData["claimed_pins"].begin(), actualData["claimed_pins"].end());
    //on enregistre les aiguillages
    for (auto it = actualData["aiguillages"].begin(); it != actualData["aiguillages"].end(); ++it)
    {
        if (it->operator[]("type") == BaseAiguillage::SimpleAiguillage1)
        {
            nlohmann::json saved = *it;
            m_aiguillages.push_back(std::shared_ptr<BaseAiguillage> (new SimpleAiguillage1(saved, shared_from_this())));
        }
    }

    //on enregistre les alimentations :
    for(auto it = actualData["alimentations"].begin(); it != actualData["alimentations"].end(); ++it)
    {
        nlohmann::json thisData = *it;
        m_alimentations.push_back(std::shared_ptr<Alimentation>(new Alimentation({thisData["pin_id"], thisData["aiguillage_handler_id"], thisData["alimentation_id"], thisData["alimentation_name"], thisData["state"]})));
    }


}

void DirectAiguillageHandler::registerJSonNode()
{
    m_serializer.registerJSonNode([=](AiguillageHandler* toSerialize)
    {
        nlohmann::json toReturn;
        DirectAiguillageHandler* toSerializeCasted = dynamic_cast<DirectAiguillageHandler*>(toSerialize);
        toReturn["claimed_pins"] = nlohmann::json::array();
        for (auto it = toSerializeCasted->claimedPins.begin(); it != toSerializeCasted->claimedPins.end(); ++it)
        {
            toReturn["claimedPins"].push_back(*it);
        }
        toReturn["aiguillages"] = nlohmann::json::array();
        for (auto it = toSerializeCasted->m_aiguillages.begin(); it != toSerializeCasted->m_aiguillages.end(); ++it)
        {
            toReturn["aiguillages"].push_back((*it)->save());
        }
        toReturn["alimentations"] = nlohmann::json::array();
        for (auto it = m_alimentations.begin(); it != m_alimentations.end(); ++it)
        {
            nlohmann::json alim;

            alim["pin_id"] = (*it)->pinId;
            alim["aiguillage_handler_id"] = (*it)->aiguillageHandlerId;
            alim["alimentation_id"] = (*it)->alimentationId;
            alim["alimentation_name"] = (*it)->alimentationName;
            alim["state"] = (*it)->state;
            toReturn["alimentations"].push_back(alim);
        }

        return toReturn;
    }, "DirectAiguillageHandler");
}

DirectAiguillageHandler::~DirectAiguillageHandler()
{
    //dtor
}

void DirectAiguillageHandler::launch()
{
    std::cout<<"Launching DirectAiguillageHandler thread..."<<std::endl;
    m_aiguillageChecker = std::shared_ptr<std::thread>(new std::thread(&DirectAiguillageHandler::t_aiguillageChecker,this));
}

DirectAiguillageHandler::AiguillageHandlerActivatingAiguillageState DirectAiguillageHandler::switchAiguillage(int aiguillageId, BaseAiguillage::Direction direction)
{

    std::shared_ptr<BaseAiguillage> toSwitch = findAiguillageById(aiguillageId).second.lock();
    //1. V�rifier si la direction est valide
    std::vector<BaseAiguillage::Direction> validDirections = toSwitch->getValidDirections();
    bool validDirection = false;
    for (auto it = validDirections.begin(); it != validDirections.end(); ++it)
    {
        if (*it == direction)
            validDirection = true;
    }
    if(validDirection == false)
        return DirectAiguillageHandler::AiguillageHandlerActivatingAiguillageState::AiguillageInvalidSensError;


    std::vector<BaseAiguillage::ErrorsAiguillage> errors;
    toSwitch->changeSens(direction, errors);
    return AiguillageHandlerActivatingAiguillageState::AiguillageDone;
}

DirectAiguillageHandler::AiguillageHandlerSwitchingAlimState DirectAiguillageHandler::toggleAlim(int alimId, bool switchState)
{
    int pinAlim = 0;
    for (auto it = m_alimentations.begin(); it != m_alimentations.end(); ++it)
    {
        if ((*it)->alimentationId == alimId)
            pinAlim = (*it)->pinId;
    }
#ifdef RASP

    if (switchState == true)
    {
        digitalWrite(pinAlim, HIGH);
        std::cout<<"[DEBUG] Pin "<<pinAlim<<" activ� (alim)"<<std::endl;
    }
    else
    {
        digitalWrite(pinAlim, LOW);
        std::cout<<"[DEBUG] Pin "<<pinAlim<<" d�sactiv� (alim)"<<std::endl;
    }


#endif // RASP
#ifndef RASP
    if (switchState == true)
        std::cout<<"Alimentation "<<pinAlim<< " activee."<<std::endl;

    else
        std::cout<<"Alimentation "<<pinAlim<< " desactivee."<<std::endl;
#endif // RASP
}

bool DirectAiguillageHandler::claimPin(int pinToClaim)
{
#ifdef RASP

    pinMode(pinToClaim, OUTPUT);
    digitalWrite(pinToClaim, LOW);

#endif // RASP
    if (!isPinClaimed(pinToClaim))
    {
        claimedPins.push_back(pinToClaim);
        return true;
    }
    else
        return false;
}

bool DirectAiguillageHandler::isPinClaimed(int pin)
{
    for (auto it = claimedPins.begin(); it != claimedPins.end(); ++it)
    {
        if (pin == *it)
            return true;
    }
    return false;
}

bool DirectAiguillageHandler::unclaimPin(int pinToUnclaim)
{
    if (isPinClaimed(pinToUnclaim))
    {
        for (auto it = claimedPins.begin(); it != claimedPins.end(); ++it)
        {
            if (*it == pinToUnclaim)
            {
                claimedPins.erase(it);
                return true;
            }
        }
    }
    return false;
}

void DirectAiguillageHandler::simpleSwitch(int pin, SimpleAiguillage::PinState sens)
{
#ifndef RASP
    std::cout<<"Pin "<<pin<<" switch � la position "<<sens<<std::endl;
#endif // RASP

#ifdef RASP
    int position;
    if (sens == SimpleAiguillage::PinState::Activated)
    {
        std::cout<<"[DEBUG] pin "<<pin<<" activ�"<<std::endl;
        position = HIGH;
    }
    else if (sens == SimpleAiguillage::PinState::Unactivated)
    {
        std::cout<<"[DEBUG] pin "<<pin<<" d�sactiv�"<<std::endl;
        position = LOW;
    }
    digitalWrite(pin, position);
#endif // RASP
}

void DirectAiguillageHandler::doubleSwitch(int pin)
{
#ifndef RASP
    std::cout<<"pin "<<pin<<" active"<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(SWITCH_TIMER));
    std::cout<<"pin "<<pin<<" desactive"<<std::endl;
#endif // RASP

#ifdef RASP
    digitalWrite(pin, HIGH);
    std::this_thread::sleep_for(std::chrono::milliseconds(SWITCH_TIMER));
    digitalWrite(pin, LOW);
    std::cout<<"[DEBUG] Pin "<<pin<<" DoubleSwitch�"<<std::endl;
#endif // RASP
}

std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage> > > DirectAiguillageHandler::getAiguillages()
{
    std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage> > >aiguillages;
    for (auto it = m_aiguillages.begin(); it != m_aiguillages.end(); it++)
    {

        aiguillages.push_back(std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>(nlohmann::json({{"testParam",""}}),*it));
    }
    return aiguillages;
}
std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>> DirectAiguillageHandler::findAiguillageById(int id, bool* founded)
{
    for (auto it = m_aiguillages.begin(); it!= m_aiguillages.end(); it++)
    {
        if ((*it)->getId() == id)
        {
            if (founded != nullptr)
                *founded = true;
            return std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>> (nlohmann::json(),*it);
        }
    }
    if (founded != nullptr)
        *founded = false;
    std::cout<<"recherche d'aiguillage �chou�e"<<std::endl;
    return std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>(nlohmann::json(),std::weak_ptr<BaseAiguillage>());

}
void DirectAiguillageHandler::addAiguillage(std::shared_ptr<BaseAiguillage> aiguillage, nlohmann::json comp)
{

    std::vector<int> usedPinsByAiguillage = aiguillage->getUsedPins();
    auto alreadyUsedPins = getUsedPins();
    //1. test si le pin est utilis� ou pas
    for (auto it = usedPinsByAiguillage.begin(); it != usedPinsByAiguillage.end(); ++it)
    {
        for (auto it2 = alreadyUsedPins.begin(); it2 != alreadyUsedPins.end(); ++it2)
        {
            if (*it2 == *it)
            {
                std::shared_ptr<AiguillageHandlerEvent> event (new AiguillageHandlerEvent(AiguillageHandlerEvent::AiguillageHandlerEventTypes::AiguillageAdded, 0));
                event->aiguillageAddedEvent.done = false;
                event->aiguillageAddedEvent.errorType = AiguillageHandlerEvent::AiguillageAddedEvent::PinAlreadyUsed;
                //callback->push_back(event);
                m_parent->proceedEvent(event);
                return;
            }
        }
    }

    //2. tester si les pins sont valides
    for (auto it = usedPinsByAiguillage.begin(); it != usedPinsByAiguillage.end(); ++it)
    {
        if (*it < 1 && *it > 29)
        {
            std::shared_ptr<AiguillageHandlerEvent> event (new AiguillageHandlerEvent(AiguillageHandlerEvent::AiguillageHandlerEventTypes::AiguillageAdded, 0));
            event->aiguillageAddedEvent.done = false;
            event->aiguillageAddedEvent.errorType = AiguillageHandlerEvent::AiguillageAddedEvent::ErrorTypes::PinInvalid;
            //callback->push_back(event);
            m_parent->proceedEvent(event);
            return;
        }
    }

    //On est bon, on ajoute l'aiguillage
    for (auto it = usedPinsByAiguillage.begin(); it != usedPinsByAiguillage.end(); ++it)
    {
        claimPin(*it);
    }
    std::cout<<aiguillage->getTag()<<std::endl;
    aiguillage->setId(getNewAiguillageId());
    m_aiguillages.push_back(aiguillage);
    std::shared_ptr<AiguillageHandlerEvent> event (new AiguillageHandlerEvent(AiguillageHandlerEvent::AiguillageHandlerEventTypes::AiguillageAdded));
    event->aiguillageAddedEvent.done = true;
    event->aiguillageAddedEvent.aiguillageHandlerId = aiguillage->getParentAiguillageHandler().lock()->getId();
    event->aiguillageAddedEvent.aiguillageId = aiguillage->getId();
    m_parent->proceedEvent(event);

}

void DirectAiguillageHandler::addAlimentation(int pinId, std::string alimentationName, nlohmann::json comp)
{
    auto alreadyUsedPins = getUsedPins();
    //v�rifier si le pin est d�j� utilis� ou pas.


    std::shared_ptr<Alimentation> toAdd (new Alimentation);
    toAdd->alimentationName = alimentationName;
    toAdd->pinId = pinId;
    toAdd->state = 0;
    toAdd->alimentationId = getNewAlimentationId();
    toAdd->aiguillageHandlerId = this->m_id;
    claimPin(pinId);
    m_alimentations.push_back(toAdd);

    std::shared_ptr<AiguillageHandlerEvent> event(new AiguillageHandlerEvent(AiguillageHandlerEvent::AiguillageHandlerEventTypes::AlimentationAdded, 0));
    event->alimentationAddedEvent.done = true;
    event->alimentationAddedEvent.alimentationId = m_alimentations.back()->alimentationId;
    event->alimentationAddedEvent.aiguillageHandlerId = this->m_id;
    m_parent->proceedEvent(event);
}

std::vector<int> DirectAiguillageHandler::getUsedPins()
{
    std::vector<int> usedPins;
    for (auto it = m_aiguillages.begin(); it != m_aiguillages.end(); ++it)
    {
        auto aiguillageUsedPins = (*it)->getUsedPins();
        usedPins.insert(usedPins.end(), aiguillageUsedPins.begin(), aiguillageUsedPins.end());
    }
    for (auto it = m_alimentations.begin(); it != m_alimentations.end(); ++it)
    {
        usedPins.push_back((*it)->pinId);
    }
    return usedPins;
}

int DirectAiguillageHandler::getNewAlimentationId()
{
    i_nextAvailableId++;
    return i_nextAvailableId;
}

int DirectAiguillageHandler::getNewAiguillageId()
{
    i_nextAvailableAiguillageId++;
    return i_nextAvailableAiguillageId;
}

std::vector<std::pair<nlohmann::json,std::weak_ptr<SinglePinModeCompatibleAiguillageHandler::Alimentation> > > DirectAiguillageHandler::getAlimentations()
{
    std::vector<std::pair<nlohmann::json,std::weak_ptr<SinglePinModeCompatibleAiguillageHandler::Alimentation> > > alims;
    for (auto it = m_alimentations.begin(); it != m_alimentations.end(); ++it)
    {
        alims.push_back(std::pair<nlohmann::json,std::weak_ptr<SinglePinModeCompatibleAiguillageHandler::Alimentation>>(nlohmann::json(),*it));
    }
    return alims;
}

nlohmann::json DirectAiguillageHandler::getCompLocParamsAiguillage()
{
    /*
    return nlohmann::json();//pas de param�tre n�cessaire suppl�mentaire
    */
    nlohmann::json toReturn;
    toReturn["testParam"] = "";
    toReturn["testParam2"] = "bouh";
    return toReturn;
}

nlohmann::json DirectAiguillageHandler::getCompLocParamsAlimentation()
{
    return nlohmann::json();
}

void DirectAiguillageHandler::t_aiguillageChecker()
{
    while (true)
    {
        for (auto it = m_aiguillages.begin(); it != m_aiguillages.end(); ++it)
        {
            (*it)->directionChanger();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void DirectAiguillageHandler::transmitEvent(std::shared_ptr<AiguillageHandlerEvent>event)
{
    m_parent->proceedEvent(event);
}

void DirectAiguillageHandler::requestSimpleSwitch(SimpleAiguillage::SinglePinModePin toSwitch)
{
    simpleSwitch(toSwitch.pinNumber,toSwitch.pinState);
}
