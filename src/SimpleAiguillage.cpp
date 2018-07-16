#include "SimpleAiguillage.h"
#include "AiguillageManager.h"
#include "SinglePinModeCompatibleAiguillageHandler.h"

SimpleAiguillage::SimpleAiguillage(std::vector<Direction>validDirections) : BaseAiguillage(validDirections)
{

}

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
        sf::sleep(sf::seconds(1));

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
