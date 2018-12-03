#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <iostream>
#include <string>
#include "json.hpp"
#include <memory>
#include <functional>
#include "EventArgs.h"

class EventListener
{
    public:
        EventListener();
        virtual ~EventListener();
        void subscribe(std::string eventName, std::function<void(std::shared_ptr<EventArgs> = std::shared_ptr<EventArgs>())> callback);
        void emit(std::string eventName, std::shared_ptr<EventArgs> args = std::shared_ptr<EventArgs>());
        void unsuscribe(std::string eventName, std::function<void(std::shared_ptr<EventArgs> = std::shared_ptr<EventArgs>())> toDelete);

    protected:

    private:
};

#endif // EVENTLISTENER_H
