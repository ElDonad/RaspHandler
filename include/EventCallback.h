#ifndef EVENTCALLBACK_H
#define EVENTCALLBACK_H

#include <functional>
#include <string>
#include <memory>

#include "EventArgs.h"

class EventCallback
{
    public:
        EventCallback(std::function<void(std::shared_ptr<EventArgs> = std::shared_ptr<EventArgs>())>);
        virtual ~EventCallback();

        inline bool operator==( const EventCallback& b)const {
            return (b.m_id == m_id);
        }
    protected:
        std::function<void(std::shared_ptr<EventArgs>)> m_function;
        int m_id;
        static int m_nextAvailableId;
};



#endif // EVENTCALLBACK_H
