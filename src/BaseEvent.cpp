#include "BaseEvent.h"

using namespace std;

BaseEvent::BaseEvent(BaseEvent::EventTypes type, int id)
{
    m_type = type;
    m_eventId = id;
}

BaseEvent::BaseEvent(BaseEvent::EventTypes type,int id, string param1String)
{
    switch (type)
    {


        case EventTypes::RemovingAiguillageConfirmed :
            {
                BaseEvent::removingAiguillageConfirmedEvent = RemovingAiguillageConfirmedEvent();
                BaseEvent::removingAiguillageConfirmedEvent.tag = param1String;
                break;
            }

        case EventTypes::Stopping :
            {
                BaseEvent::stoppingEvent = StoppingEvent();
                BaseEvent::stoppingEvent.raison = param1String;
                break;
            }

        default :
            break;
    }
    m_type = type;
    m_eventId = id;
}

BaseEvent::BaseEvent(BaseEvent::EventTypes type, int id, string param1String, int param2Int)
{
    switch (type)
    {

        case EventTypes::ChangingAiguillageStateConfirmed :
            {
                BaseEvent::changingAiguillageStateConfirmedEvent = ChangingAiguillageStateConfirmedEvent();
                changingAiguillageStateConfirmedEvent.tag = param1String;
                changingAiguillageStateConfirmedEvent.newDirection = param2Int;
                break;
            }

        default :
            break;
    }
    m_type = type;
    m_eventId = id;

}

BaseEvent::BaseEvent(EventTypes type, int id, vector<string> params)
{
    switch (type)
    {
    case EventTypes::AddingAiguillageAborted :
        {
            BaseEvent::addingAiguillageAbortedEvent = AddingAiguillageAbortedEvent();
            BaseEvent::addingAiguillageAbortedEvent.errors = params;
            break;
        }

    case EventTypes::RemovingAiguillageAborted :
        {
            BaseEvent::removingAiguillageAbortedEvent = RemovingAiguillageAbortedEvent();
            removingAiguillageAbortedEvent.errors = params;
            break;
        }

//        Problème de constructeur refait...
//    case EventTypes::ChangingAiguillageStateAborted :
//        {
//            BaseEvent::changingAiguillageStateAbortedEvent =  ChangingAiguillageStateAbortedEvent();
//            changingAiguillageStateAbortedEvent.tag = params[0];
//            for (int loop = 1; loop != params.size(); loop++)
//            {
//                changingAiguillageStateAbortedEvent.errors.push_back(params[loop]);
//            }
//            break;
//        }
    default:
        break;
    }
    m_type = type;
    m_eventId = id;
}

BaseEvent::~BaseEvent()
{

}

BaseEvent::EventTypes BaseEvent::getType()
{
    return m_type;
}

int BaseEvent::getId()
{
    return m_eventId;
}


