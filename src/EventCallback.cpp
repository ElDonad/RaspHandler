#include "EventCallback.h"

int EventCallback::m_nextAvailableId = 0;

EventCallback::EventCallback(std::function<void(std::shared_ptr<EventArgs> = std::shared_ptr<EventArgs>())> function)
{
    m_function = function;
    m_id = m_nextAvailableId;
    m_nextAvailableId++;
}

EventCallback::~EventCallback()
{
    //dtor
}
