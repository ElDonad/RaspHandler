#include "ConsoleUserHandler.h"
#include "EventHandler.h"
#include "BaseAiguillage.h"
#include "json.hpp"

using namespace std;

ConsoleUserHandler::ConsoleUserHandler(EventHandler* parent)
{
    m_listenerThread = new sf::Thread(ConsoleUserHandler::m_listener, this);
    m_handlerThread = new sf::Thread(ConsoleUserHandler::m_handler, this);
    m_stop = false;
    m_parentEventHandler = parent;
}

ConsoleUserHandler::~ConsoleUserHandler()
{
    //dtor
}

void ConsoleUserHandler::m_listener()
{
    while (true)
    {
        cin>>m_entry;
        m_Mentry.lock();
        m_entryChanged = true;
        m_Mentry.unlock();
    }
}

void ConsoleUserHandler::m_handler()
{
    while (m_stop == false)
    {
        //Menuu principal :
        cout<<endl<<"> Menu principal :"<<endl;
        cout<<">1. Consulter la liste des aiguillages"<<endl;
        cout<<">2. Ajouter ou supprimer un aiguillage"<<endl;
        cout<<">3. Manipuler les aiguillages"<<endl;
        cout<<">4. Gestionnaire d'Alimentations"<<endl;
        cout<<">5. Parametres debug"<<endl;
        cout<<">x. Quitter"<<endl;
        string retour = safeWaitForEntry();

        if (retour == "1")//liste
        {

            displayAiguillagesList();
            std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>> aiguillages = m_parentEventHandler->getAiguillage();
            cout<<endl<<"> Pour en savoir plus sur un aiguillage, tapez son numéro d'identifiant. sinon, tapez 'x'. ";
            bool quit = false;
            while (quit == false)
            {
                retour = safeWaitForEntry();
                try
                {
                    int result = std::stoi(retour);
                    auto it3 = aiguillages.begin();
                    for (int loopDeMerde = 0; loopDeMerde != result - 1; ++loopDeMerde)
                        ++it3;

                    nlohmann::json compDonnees = it3->first;
                    for (auto it4 = compDonnees.begin(); it4 != compDonnees.end(); ++it4)
                    {
                        cout<<it4.key()<<" : "<<it4.value()<<endl;
                    }
                }
                catch(std::invalid_argument)
                {
                    if (retour == "x")
                        quit = true;
                    else{
                        cout<<endl<<"Abruti regarde tes doigts quand tu tapes !!"<<endl;
                        continue;
                    }

                }
            }


            continue;
        }

        else if (retour == "2")//ajouter ou supprimer des aiguillages
        {
            cout<<"> Entrez le type de l'aiguillage (se référer à la documentation)"<<endl;
            string retour = safeWaitForEntry();

            //Liste des aiguillages possinles
            if (retour == "SimpleAiguillage1")
            {
                string nom;
                cout<<"> Entrez le nom de l'aiguillage : ";
                nom = safeWaitForEntry();
                int pin;
                cout<<endl<<"> Entrez le pin de l'aiguillage : ";
                pin = stoi(safeWaitForEntry());
                cout<<endl<<"> Entrez la direction de base de l'aiguillage (pin désactivé)(g/d) : ";
                string direction = "";

                while (direction != "g" && direction != "d")
                    {
                        cout<<endl<<"Entrez la direction : ";
                        direction = safeWaitForEntry();

                    }

                SimpleAiguillage1::SinglePinModePin left, right;
                left.pinNumber = right.pinNumber = pin;
                if (direction == "g")
                {
                    left.pinState = SimpleAiguillage::PinState::Unactivated;
                    right.pinState = SimpleAiguillage::PinState::Activated;
                }
                else if (direction == "d")
                {
                    left.pinState = SimpleAiguillage::PinState::Activated;
                    right.pinState = SimpleAiguillage::PinState::Unactivated;
                }
                std::shared_ptr<UserEvent> aiguillageToAdd (new UserEvent(UserEvent::UserEventTypes::AddAiguillage));
                displayAiguillageHandlersList();
                int aiguillageHandler;
                cout<<"Entrez l'id de l'aiguilageHandler : ";

                aiguillageHandler = stoi(safeWaitForEntry());
                aiguillageToAdd->addAiguillageEvent.idAiguillageHandler = aiguillageHandler;
                std::shared_ptr<AiguillageHandler>ptrAiguillageHandler = m_parentEventHandler->getAiguillageHandlerById(aiguillageHandler).lock();
                cout<<endl;
                displayAlimentationsList(ptrAiguillageHandler->getId());
                bool done = false;
                int alimId;
                while (done == false)
                {
                    try
                    {
                        cout<<endl<<"Entrez l'id de l'alimentation correspondante : ";
                        alimId = std::stoi(safeWaitForEntry());
                        done = true;
                    }
                    catch(invalid_argument)
                    {
                        cout<<endl<<"Tu pourrais la refaire en te gourant pas s'il te plaît ?"<<endl;
                    }
                }
                cout<<endl;
                int alimHandlerId = getSafeInt("> Entrez l'id du handler de l'alimentation");


                nlohmann::json toFill = ptrAiguillageHandler->getCompLocParamsAiguillage();
                for (auto it = toFill.begin(); it != toFill.end(); ++it)
                {
                    if (it->is_boolean())
                    {
                        bool done = false;
                        while (done == false)
                        {
                            cout<<endl<<it.key()<<" (y/n) : ";
                            string reponse = safeWaitForEntry();
                            if (reponse == "y")
                                toFill[it.key()] = true;
                            else if (reponse == "n")
                                toFill[it.key()] = false;
                            else
                                continue;
                        }
                        cout<<endl;

                    }
                    else if (it->is_number())
                    {
                        cout<<it.key()<<" : ";
                        bool done = false;
                        while (done == false)
                        {
                            string reponse = safeWaitForEntry();
                            try{
                                toFill[it.key()] = stod(reponse);}
                            catch(invalid_argument)
                            {
                                cout<<endl<<"Surveille ce que tu tapes, pls ! "<<endl;
                                continue;
                            }
                        }
                        cout<<endl;
                    }
                    else if(it->is_string())
                    {
                        cout<<it.key()<<" : ";
                        toFill[it.key()] = safeWaitForEntry();
                        cout<<endl;
                    }
                    else
                    {
                        cout<<"Bug de typage de donnée !!"<<endl;
                    }
                }

                SimpleAiguillage1::AiguillageParams* params = new SimpleAiguillage1::AiguillageParams();
                params->direction = SimpleAiguillage1::Direction::Left;
                params->leftDirectionPin = left;
                params->rightDirectionPin = right;
                params->name = nom;
                params->alimId = alimId;
                params->alimHandlerId = alimHandlerId;
                aiguillageToAdd->addAiguillageEvent.type = BaseAiguillage::AiguillageType::SimpleAiguillage1;
                aiguillageToAdd->addAiguillageEvent.params = params;
                aiguillageToAdd->addAiguillageEvent.tag = nom;
                aiguillageToAdd->addAiguillageEvent.comp = toFill;
                m_parentEventHandler->transmitEvent(aiguillageToAdd);
                cout<<"> done"<<endl;

            }
        }

        else if (retour == "3")//manipuler les aiguillages
        {
            displayAiguillagesList();
            int aiguillageId = getSafeInt("Entrez l'aiguillage à manipuler : ");
            std::weak_ptr<BaseAiguillage> wk_toSwitch = m_parentEventHandler->getAiguillage()[aiguillageId - 1].second;
            nlohmann::json toSwitchComp = m_parentEventHandler->getAiguillage()[aiguillageId - 1].first;
            std::shared_ptr<BaseAiguillage> toSwitch = wk_toSwitch.lock();
            cout<<endl<<">Aiguillage choisi : "<<toSwitch->getTag()<<endl;
            std::vector<BaseAiguillage::Direction> allowedDirections = toSwitch->getValidDirections();
            cout<<endl<<"Liste des directions autorisées : "<<endl;
            int directionId = 1;
            for (auto it = allowedDirections.begin(); it != allowedDirections.end(); ++it)
            {
                cout<<directionId<<". ";
                switch(*it)
                {
                case BaseAiguillage::Direction::Left:
                    {
                        cout<<"Gauche";
                        break;
                    }
                case BaseAiguillage::Direction::Middle:
                    {
                        cout<<"Milieu";
                        break;
                    }
                case BaseAiguillage::Direction::Right:
                    {
                        cout<<"Droite";
                        break;
                    }
                default:
                    {
                        cout<<"Inconnue ou invalide"<<endl;
                        break;
                    }

                }
                cout<<endl;
                ++directionId;
            }
            cout<<endl;
            directionId = getSafeInt("Entrez la direction souhaitée : ") - 1;
            std::shared_ptr<UserEvent> event (new UserEvent(UserEvent::UserEventTypes::ChangeAiguillageState));
            std::cout<<toSwitch->getTag();
            event->changeAiguillageStateEvent.aiguillageHandlerId = toSwitch->getParentAiguillageHandler().lock()->getId();
            event->changeAiguillageStateEvent.aiguillageId = toSwitch->getId();
            event->changeAiguillageStateEvent.targetState = toSwitch->getValidDirections()[directionId];
            m_parentEventHandler->transmitEvent(event);
            cout<<">Done"<<endl;

        }

        else if (retour == "4")//Gestionnaire d'Alimentation
        {
            cout<<"     ++Gestionnaire d'alimentation++     "<<endl<<endl;
            cout<<">1. Consulter la liste des alimentations"<<endl;
            cout<<">2. Ajouter une alimentaion"<<endl;
            cout<<">3. Supprimer une alimentation(les aiguillages qui en dépendent deviendront indisponibles"<<endl;
            retour = safeWaitForEntry();
            if (retour == "1")//consulter la liste des alimentations
            {
                cout<<endl<<">Liste des alimentations : "<<endl<<endl;
                displayAlimentationsList();

                retour = "0";

            }
            else if (retour == "2")//ajouter une alimentation
            {
                cout<<"Liste des EventHandlers..."<<endl;
                displayAiguillageHandlersList();
                std::shared_ptr<UserEvent> event (new UserEvent(UserEvent::UserEventTypes::AddAlimentation));
                m_storedEventId.push_back(event->getId());

                //récupération de l'id de l'eventHandler
                bool done = false;
                while (done == false)
                {
                    try
                    {
                        cout<<">Sélectionnez l'EventHandler hôte en inscrivant son id : ";
                        retour = safeWaitForEntry();
                        event->addAlimentationEvent.idAiguillageHandler = stoi(retour);
                        done = true;
                    }
                    catch(invalid_argument)
                    {
                        cout<<endl<<"Andouille, tu écris n'importe quoi ! recommence !"<<endl;
                        continue;
                    }
                }
                cout<<endl;
                done = false;
                while (done == false)
                {
                    try
                    {
                        cout<<"Entrez le pin utilisé par l'alimentation : ";
                        retour = safeWaitForEntry();
                        event->addAlimentationEvent.pinAlim = stoi(retour);
                        done = true;
                    }
                    catch(invalid_argument)
                    {
                        cout<<endl<<"Fait un peu gaffe, enfin !"<<endl;
                    }
                }
                cout<<endl<<"Entrez le nom de l'alimentation :"<<endl;
                event->addAlimentationEvent.alimentationName = safeWaitForEntry();


                //paramètres supplémentaires...
                std::shared_ptr<AiguillageHandler> handler = m_parentEventHandler->getAiguillageHandlerById(event->addAlimentationEvent.idAiguillageHandler).lock();
                std::shared_ptr<SinglePinModeCompatibleAiguillageHandler> goodHandler = std::dynamic_pointer_cast<SinglePinModeCompatibleAiguillageHandler>(handler);
                nlohmann::json toFill = goodHandler->getCompLocParamsAlimentation();
                for (auto it = toFill.begin(); it != toFill.end(); ++it)
                {
                    if (it->is_boolean())
                    {
                        bool done = false;
                        while (done == false)
                        {
                            cout<<endl<<it.key()<<" (y/n) : ";
                            string reponse = safeWaitForEntry();
                            if (reponse == "y")
                                toFill[it.key()] = true;
                            else if (reponse == "n")
                                toFill[it.key()] = false;
                            else
                                continue;
                        }
                        cout<<endl;

                    }
                    else if (it->is_number())
                    {
                        cout<<it.key()<<" : ";
                        bool done = false;
                        while (done == false)
                        {
                            string reponse = safeWaitForEntry();
                            try{
                                toFill[it.key()] = stod(reponse);}
                            catch(invalid_argument)
                            {
                                cout<<endl<<"Gros nul fais attention !";
                                continue;
                            }
                        }
                        cout<<endl;
                    }
                    else if(it->is_string())
                    {
                        cout<<it.key()<<" : ";
                        toFill[it.key()] = safeWaitForEntry();
                        cout<<endl;
                    }
                    else
                    {
                        cout<<"Bug de typage de donnée !!"<<endl;
                    }
                }
                event->addAiguillageEvent.comp = toFill;
                m_parentEventHandler->transmitEvent(event);
                cout<<">Done !"<<endl<<endl;
                continue;

            }

        }

//
//        else if (retour == "3")//modifier sens
//        {
//            cout<<"> Sélectionnez un aiguillage :"<<endl;
//            cout<<"Num|        Nom         |Dir"<<endl;
//            vector <Aiguillage*> aiguillages = m_parentEventHandler->getAiguillage();
//            for (int loop = 0; aiguillages.size() != loop; loop++)
//            {
//                string nomAiguillage = formatWithSpaces(20,aiguillages[loop]->getTag());
//
//                string direction;
//                if (aiguillages[loop]->getDirection() == Aiguillage::Left)
//                {
//                    direction = " G ";
//                }
//                if (aiguillages[loop]->getDirection() == Aiguillage::Right)
//                {
//                    direction = " D ";
//                }
//                string id = to_string(loop + 1);
//                while (id.size() < 3)
//                {
//                    id = id + " ";
//                }
//
//
//                cout<<id<<"|"<<nomAiguillage<<"|"<<direction<<endl;
//
//            }
//            retour = safeWaitForEntry();
//            int intRetour = stoi(retour);
//            for (int loop = 0; loop != aiguillages.size(); loop++)
//            {
//                if (loop == intRetour - 1)
//                {
//                    cout<<"Entrez la direction : ";
//                    retour = safeWaitForEntry();
//                    if (retour == "G" || retour == "4" || retour == "g")
//                    {
//                        vector<string> params;
//                        params.push_back(aiguillages[loop]->getTag());
//                        params.push_back("0");
//                        UserEvent changeSens(UserEvent::ChangeAiguillageState, params);
//                        m_parentEventHandler->transmitEvent(changeSens);
//                        cout<<endl<<"Requête envoyée !"<<endl;
//                    }
//                    else if (retour == "D" || retour == "6" || retour == "d")
//                    {
//                        vector<string> params;
//                        params.push_back(aiguillages[loop]->getTag());
//                        params.push_back("1");
//                        UserEvent changeSens(UserEvent::ChangeAiguillageState, params);
//                        m_parentEventHandler->transmitEvent(changeSens);
//                        cout<<endl<<"Requête envoyée !"<<endl;
//                    }
//                    else if (retour == "|abort")
//                        return;
//                }
//            }
//        }
//
//        else if (retour == "4")
//        {
//            cout<<"> Activer/desactiver le debug ?"<<endl;
//            string retour;
//            cin>>retour;
//            if (retour == "1")
//                m_isInDebugMode = true;
//            else if (retour == "0")
//                m_isInDebugMode = false;
//        }

        else if (retour == "x")
        {
            cout<<"Retour demandé"<<endl;
            std::shared_ptr<UserEvent> stopEvent(new UserEvent(UserEvent::Stop));
            stopEvent->stopEvent.raison = "Arrêt initié par le client console";
            m_parentEventHandler->transmitEvent(stopEvent);
        }
        else if (retour == "|abort")
            return;
    }
}

string ConsoleUserHandler::safeWaitForEntry()
{
    m_Mentry.lock();
    m_entryChanged = false;
    m_Mentry.unlock();
    while (m_entryChanged == false && m_stop == false)
    {
        sf::sleep(sf::milliseconds(2));
    }
    if(m_stop == false)
    {
        return m_entry;
    }
    return "|abort";
}

void ConsoleUserHandler::receiveAnswer(std::shared_ptr<BaseEvent> answer)
{
    switch (answer->getType())
    {
    case BaseEvent::AddingAiguillageAborted:
        {
            if (isConcerned(answer->getId()) == true)
            {
                cout<<"> L'ajout de l'aiguillage a échoué pour les raisons suivantes :"<<endl;
                for (int loop = 0; loop != answer->addingAiguillageAbortedEvent.errors.size(); loop++)
                {
                    cout<<" - "<<answer->addingAiguillageAbortedEvent.errors[loop]<<endl;
                }
            }
        }

    case BaseEvent::AddingAiguillageConfirmed:
        {
            if (isConcerned(answer->getId()) == true)
            {
                cout<<"Aiguillage "<<m_parentEventHandler->findAiguillageById(answer->addingAiguillageConfirmedEvent.aiguillageHandlerId,answer->addingAiguillageConfirmedEvent.aiguillageId, nullptr).second.lock()->getTag()<<" ajouté avec succès"<<endl;
            }
            break;

        }

    case BaseEvent::AiguillageAdded:
        {
            cout<<"Aiguillage "<<m_parentEventHandler->findAiguillageById(answer->aiguillageAddedEvent.handlerId, answer->aiguillageAddedEvent.aiguillageId, nullptr).second.lock()->getTag()<<" ajouté."<<endl;
            break;
        }

    }
}

bool ConsoleUserHandler::isConcerned(int idToConfirm)
{
    for (int loop = 0; loop != m_storedEventId.size(); loop++)
    {
        if (m_storedEventId[loop] == idToConfirm)
            return true;
    }
    return false;
}

void ConsoleUserHandler::launch()
{
    m_handlerThread->launch();
    m_listenerThread->launch();
}

void ConsoleUserHandler::stop()
{
    m_stop = true;
    m_listenerThread->terminate();
    m_handlerThread->wait();
}

string ConsoleUserHandler::formatWithSpaces(int spaceMax, const string& nomAiguillage)
{
    string toReturn = nomAiguillage;
    while (toReturn.size() < spaceMax)
    {
        toReturn = toReturn + " ";
    }
    while (toReturn.size() > spaceMax)
    {
        toReturn.pop_back();
    }
    return toReturn;
}

void ConsoleUserHandler::displayAiguillageHandlersList()
{
    using namespace std;
    cout<<"> Affichage de la liste des aiguillageHandlers..."<<endl;
    cout<<"           NOM           | ID |"<<endl;
    auto aiguillageHandlers = m_parentEventHandler->getAiguillageHandlers();
    for(auto it = aiguillageHandlers.begin(); it != aiguillageHandlers.end(); ++it)
    {
        std::shared_ptr<AiguillageHandler> handler = it->lock();
        cout<<formatWithSpaces(25,handler->getName())<<"|"<<formatWithSpaces(4, std::to_string(handler->getId()))<<endl;
    }
}

void ConsoleUserHandler::displayAlimentationsList()
{
    cout<<"           NOM           | ID |AHID|ON"<<endl;
    std::vector<std::shared_ptr<SinglePinModeCompatibleAiguillageHandler::Alimentation>> alimentations;
    std::vector<std::weak_ptr<AiguillageHandler>> aiguillageHandlers = m_parentEventHandler->getAiguillageHandlers();
    for (auto it = aiguillageHandlers.begin(); it != aiguillageHandlers.end(); ++it)
    {
        if (it->lock()->isSinglePinModeCompatible())
        {
            std::shared_ptr<SinglePinModeCompatibleAiguillageHandler> handler = std::dynamic_pointer_cast<SinglePinModeCompatibleAiguillageHandler> (it->lock());
            std::vector<std::pair<nlohmann::json,std::weak_ptr<SinglePinModeCompatibleAiguillageHandler::Alimentation> > > weakAlims = handler->getAlimentations();
            for (auto it2 = weakAlims.begin(); it2 != weakAlims.end(); ++it2)
            {
                alimentations.push_back(it2->second.lock());
            }
        }
    }
    //Affichage des Alimentations
    for (auto it = alimentations.begin(); it != alimentations.end(); ++it)
    {
        cout<<formatWithSpaces(25,(*it)->alimentationName)<<"|";
        cout<<formatWithSpaces(4,std::to_string((*it)->alimentationId))<<"|";
        cout<<formatWithSpaces(4,std::to_string((*it)->aiguillageHandlerId))<<"|";
        cout<<(*it)->state<<endl;
    }
}

void ConsoleUserHandler::displayAlimentationsList(int aiguillageHandlerId)
{
    auto wk_handler = m_parentEventHandler->getAiguillageHandlerById(aiguillageHandlerId);
    std::shared_ptr<SinglePinModeCompatibleAiguillageHandler> handler = std::dynamic_pointer_cast<SinglePinModeCompatibleAiguillageHandler>(wk_handler.lock());
    cout<<"> Liste des alimentations pour l'aiguillageHandler "<<handler->getName()<<" :"<<endl<<endl;
    cout<<"           NOM           |PIN| ID "<<endl;
    std::vector<std::pair<nlohmann::json,std::weak_ptr<SinglePinModeCompatibleAiguillageHandler::Alimentation>>> alims = handler->getAlimentations();
    for (auto it = alims.begin(); it != alims.end(); ++it)
    {
        std::shared_ptr<SinglePinModeCompatibleAiguillageHandler::Alimentation> alim = it->second.lock();
        cout<<formatWithSpaces(25,alim->alimentationName)<<"|"<<formatWithSpaces(3,to_string(alim->pinId))<<"|"<<formatWithSpaces(4,to_string(alim->alimentationId));
    }
}

void ConsoleUserHandler::displayAiguillagesList()
{
    cout<<">Liste des aiguillages :"<<endl;
    cout<<"NUM|        Nom         |Dir|        Type      |DISPO"<<endl;
    std::vector <std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage> > >aiguillages = m_parentEventHandler->getAiguillage();

    int num = 0;
    for (auto it = aiguillages.begin(); it != aiguillages.end(); ++it)
    {
        DisplayAiguillage toAdd;
        auto toAddPointer = it->second.lock();
        toAdd.nom = formatWithSpaces(20,toAddPointer->getTag());
        BaseAiguillage::Direction directionToDisplay = toAddPointer->getCurrentDirection();
        switch (directionToDisplay)
        {
        case BaseAiguillage::Direction::Left:
            {
                toAdd.dir = "LFT";
                break;
            }
        case BaseAiguillage::Direction::Middle:
            {
                toAdd.dir = "MID";
                break;
            }
        case BaseAiguillage::Direction::Right:
            {
                toAdd.dir = "RGT";
                break;
            }
        default:
            {
                toAdd.dir = "ERR";
                break;
            }

        }

        switch(toAddPointer->getType())
        {
        case BaseAiguillage::AiguillageType::SimpleAiguillage1:
            {
                toAdd.type = formatWithSpaces(20,"Simple 2 dir");
                break;
            }

        case BaseAiguillage::AiguillageType::DoubleAiguillage2:
            {
                toAdd.type = formatWithSpaces(20,"Double 2 dirs");
                break;
            }
        }


        cout<<formatWithSpaces(3,std::to_string(num + 1))<<"|"<<toAdd.nom<<"|"<<toAdd.dir<<"|"<<toAdd.type;
        if ((*it).second.lock()->isDisponible())
            cout<<"Y";
        else
            cout<<"N";
        cout<<endl;
        ++num;
    }
}

int ConsoleUserHandler::getSafeInt(string annonce)
{
    int result;
    bool done = false;
    while (done == false)
    {
        cout<<annonce;
        try
        {
            result = std::stoi(safeWaitForEntry());
            done = true;
        }
        catch(invalid_argument)
        {
            cout<<endl<<"Faites attention enfin !"<<endl;
        }
    }
    return result;
}
