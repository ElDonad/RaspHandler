#ifndef USERHANDLER_H
#define USERHANDLER_H

#include <iostream>
#include <vector>
#include "BaseEvent.h"
#include <mutex>

class EventHandler;

/** Une classe virtuelle dont héritent toutes les interfaces utilisateur.
 *
 *
 *
 */

class UserHandler
{
    public:
        /** Default constructor */
        UserHandler();
        /** Default destructor */
        virtual ~UserHandler();
        virtual void receiveAnswer (std::shared_ptr<BaseEvent> answer)=0;
        virtual void launch()=0;
        virtual void stop();
        virtual nlohmann::json save()=0;

    protected:
        void requestAddAiguillage(std::string tag, int pinG, int pinD);
        void requestRemovingAiguillage(std::string tag);
        void requestConfirm();
        void requestStop(std::string raison);
        bool m_stop;

        bool isStopping();
        std::mutex m_MisStopping;
        EventHandler* m_parentEventHandler;



};

#endif // USERHANDLER_H
