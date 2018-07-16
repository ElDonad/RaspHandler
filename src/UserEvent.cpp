#include "UserEvent.h"

using namespace std;

int UserEvent::m_currentAvailableId = 0;

UserEvent::UserEvent(UserEvent::UserEventTypes type)
{
    m_userEventType = type;
    m_id = getNextAvailableId();

}

UserEvent::~UserEvent()
{
    //dtor
}

int UserEvent::getNextAvailableId()
{
    m_currentAvailableId++;
    return m_currentAvailableId - 1;
}

UserEvent::UserEventTypes UserEvent::getType()
{
    return m_userEventType;
}

int UserEvent::getId()
{
    return m_id;
}

/*UserEvent::UserEvent (UserEvent const& b)
{
    m_userEventType = b.m_userEventType;
    m_id = b.m_id;

    switch (b.m_userEventType)
    {
    case UserEvent::AddAiguillage:
        {
            addAiguillageEvent = b.addAiguillageEvent;
        }
    }
}*/
