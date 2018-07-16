#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <string>
#include <vector>
#include <SFML/System.hpp>
#include <memory>

#include "EventHandler.h"
#include "BaseEvent.h"
#include "AiguillageManager.h"


class UserEvent;
class BaseAiguillage;

class Base
{
    public:
        /** Default constructor */
        Base();
        void launch();

        /** Default destructor */
        virtual ~Base();
        void proceedEvent(std::shared_ptr<UserEvent> event);
        void transmitEvent(std::shared_ptr<BaseEvent> toTransmit);
        std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>> getAiguillages();//!<fait appel à la fonction AiguillageManager::getAiguillages()
        std::vector<std::weak_ptr<AiguillageHandler>> getAiguillageHandlers();//!<fait appel à la fonction AiguillageManager::getAiguillageHandlers()

        std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>> findAiguillageById(int aiguillageHandlerId,int aiguillageId, bool* success);//!<fait appel à la fonction AiguillageManager::
        std::weak_ptr<AiguillageHandler> findAiguillageHandlerById(int aiguillageHandlerId);

    private:
        EventHandler m_eventHandler;
        AiguillageManager m_aiguillageManager;
        bool stop;
};

#endif // BASE_H
