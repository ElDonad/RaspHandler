#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <iostream>
#include <vector>
#include <memory>

#include "UserHandler.h"
#include "AiguillageManager.h"//UNIQUEMENT pour la struct Alimentation.

#include <thread>
#include <mutex>

class Base;
class BaseEvent;
class UserEvent;
class BaseAiguillage;
class AiguillageHandler;

class EventHandler
{
    public:
        /** Default constructor */
        EventHandler(Base* parent);
        EventHandler(Base* parent, nlohmann::json save);
        /** Default destructor */
        virtual ~EventHandler();
        void eventHandlerThread();
        void sendAnswer(std::shared_ptr<BaseEvent> event);
        void setParent(Base* parent);

        void transmitEvent(std::shared_ptr<UserEvent> event);

        void stop();
        bool isStoping();
        void launch();
        nlohmann::json save();

        std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>> getAiguillage();//!< Renvoie une map qui associe un aiguillage à l'id de son AiguillageHandler.
// TODO (Elie#1#): Ajouter la fonction findAlimentationById() que j'ai oubliée... ...
//


        std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>> findAiguillageById(int aiguillageHandlerId,int aiguillageId, bool* success);
        std::weak_ptr<AiguillageHandler> getAiguillageHandlerById(int aiguillageHandlerId);
        std::vector<std::weak_ptr<AiguillageHandler>> getAiguillageHandlers();

    private:
        Base *m_base;
        std::vector <UserHandler*> m_userHandlers;
        std::thread* m_handlerThread;

        bool m_isStopping;
        std::mutex m_MIsStopping;
        std::string m_dummyString;




};

#endif // EVENTHANDLER_H
