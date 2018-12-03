#include "SimpleAiguillage.h"
#include "AiguillageManager.h"
#include "SinglePinModeCompatibleAiguillageHandler.h"

SimpleAiguillage::SimpleAiguillage(std::vector<Direction>validDirections) : BaseAiguillage(validDirections)
{
    registerJSONNode();
}

SimpleAiguillage::SimpleAiguillage(std::vector<Direction>validDirections, nlohmann::json save) : BaseAiguillage(validDirections, save)
{
    nlohmann::json actualData = save["SimpleAiguillage"];
    m_alimentationId = actualData["alimentation_id"];
    m_alimentationHandlerId = actualData["alimentation_handler_id"];

    //m_directionsMap :
    for (auto it = actualData["directions_map"].begin(); it != actualData["directions_map"].end(); ++it)
    {
        m_directionsMap.emplace(it->operator[]("direction"), SinglePinModePin(it->operator[]("pinState")["pin_number"], it->operator[]("pinState")["pin_state"]));
    }

    registerJSONNode();
}

void SimpleAiguillage::registerJSONNode()
{
 m_serializer.registerJSonNode([=](BaseAiguillage* toSerialize)
    {
        SimpleAiguillage* toSerializeCasted = dynamic_cast<SimpleAiguillage*>(toSerialize);
        nlohmann::json toReturn;

        toReturn["alimentation_id"] = toSerializeCasted->m_alimentationId;
        toReturn["alimentation_handler_id"] = toSerializeCasted->m_alimentationHandlerId;
        toReturn["directions_map"] = nlohmann::json::array();
        for(auto it = toSerializeCasted->m_directionsMap.begin(); it != toSerializeCasted->m_directionsMap.end(); ++it)
        {
            nlohmann::json toAdd;
            toAdd["direction"] = it->first;
            toAdd["pinState"] = nlohmann::json();
            toAdd["pinState"]["pin_number"] = it->second.pinNumber;
            toAdd["pinState"]["pin_state"] = it->second.pinState;

            toReturn["directions_map"].push_back(toAdd);

        }

        return toReturn;

    },"SimpleAiguillage");

    m_classicSerializer.registerJSonNode([=](BaseAiguillage* toSerialize)
    {
        nlohmann::json toReturn;

        SimpleAiguillage* toSerializeCasted = dynamic_cast<SimpleAiguillage*>(toSerialize);
        toReturn["alimentation_id"] = toSerializeCasted->m_alimentationId;
        toReturn["alimentation_handler_id"] = toSerializeCasted->m_alimentationHandlerId;
        toReturn["directions_map"] = nlohmann::json::array();
        for(auto it = toSerializeCasted->m_directionsMap.begin(); it != toSerializeCasted->m_directionsMap.end(); ++it)
        {
            nlohmann::json toAdd;
            toAdd["direction"] = it->first;
            toAdd["pinState"] = nlohmann::json();
            toAdd["pinState"]["pin_number"] = it->second.pinNumber;
            toAdd["pinState"]["pin_state"] = it->second.pinState;

            toReturn["directions_map"].push_back(toAdd);

        }

        return toReturn;
    },"SimpleAiguillage");//Ici tout est reproduit à l'identique car le client a sans doute besoin d'afficher toutes ces infos.
}

//SimpleAiguillage::SimpleAiguillage(nlohmann::json backup) : BaseAiguillage(backup)
//{
//
//}

int SimpleAiguillage::i_id = 0;

//SimpleAiguillage::~SimpleAiguillage()
//{
//    //BaseAiguillage.~BaseAiguillage();
//}

std::vector<int> SimpleAiguillage::getUsedPins()
{
    std::vector<int> pins;
    auto it = m_directionsMap.begin();
    while(it != m_directionsMap.end())
    {
        pins.push_back(it->second.pinNumber);

        it++;
    }
    return pins;
}

void SimpleAiguillage::directionChanger()
{
    std::shared_ptr<SinglePinModeCompatibleAiguillageHandler> parent = std::dynamic_pointer_cast<SinglePinModeCompatibleAiguillageHandler>(m_parent.lock());
    if (m_targetDirection != m_currentDirection)
    {
        std::shared_ptr<AiguillageHandlerEvent> onAlim (new AiguillageHandlerEvent(AiguillageHandlerEvent::AiguillageHandlerEventTypes::RequestSinglePinAlim,m_parent.lock()->getId()));
        onAlim->requestSinglePinAlimEvent.alimId= m_alimentationId;
        onAlim->requestSinglePinAlimEvent.aiguillageHandlerId = m_alimentationHandlerId;
        onAlim->requestSinglePinAlimEvent.state = true;
        parent->transmitEvent(onAlim);

        parent->requestSimpleSwitch(m_directionsMap[m_targetDirection]);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::shared_ptr<AiguillageHandlerEvent> offAlim (new AiguillageHandlerEvent(AiguillageHandlerEvent::AiguillageHandlerEventTypes::RequestSinglePinAlim,m_parent.lock()->getId()));
        offAlim->requestSinglePinAlimEvent.aiguillageHandlerId = m_alimentationHandlerId;
        offAlim->requestSinglePinAlimEvent.alimId = m_alimentationId;
        offAlim->requestSinglePinAlimEvent.state = false;
        parent->transmitEvent(offAlim);

        m_currentDirection = m_targetDirection;
    }

}

SimpleAiguillage::PinState SimpleAiguillage::getInvertedPinState(PinState toInvert)
{
    if (toInvert == Activated)
        return Unactivated;
    else if (toInvert == Unactivated)
        return Activated;
}

void SimpleAiguillage::confirm()
{

}

int SimpleAiguillage::getNextDisponibleId()
{
    i_id++;
    return i_id;
}

std::string SimpleAiguillage::getStringifiedType()
{
    return "SimpleAiguillage";
}
