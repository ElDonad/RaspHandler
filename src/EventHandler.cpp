#include "EventHandler.h"
#include "Base.h"
#include "BaseEvent.h"
#include "UserEvent.h"

#include "NetworkUserHandler.h"
#include "ConsoleUserHandler.h"

using namespace std;

EventHandler::EventHandler(Base* parent)
{
    m_base = parent;
    m_isStopping = false;
    m_userHandlers.push_back(dynamic_cast<UserHandler*>(new NetworkUserHandler(this)));
    m_userHandlers.push_back(dynamic_cast<UserHandler*>(new ConsoleUserHandler(this)));
    //m_handlerThread = new std::thread(&EventHandler::eventHandlerThread, this);

}

EventHandler::EventHandler(Base* parent, nlohmann::json save)
{
    m_base = parent;
    m_isStopping = false;

    m_userHandlers.push_back(dynamic_cast<UserHandler*>(new NetworkUserHandler(this, save["network_user_handler"])));
    m_userHandlers.push_back(dynamic_cast<UserHandler*>(new ConsoleUserHandler(this, save["console_user_handler"])));
    //m_handlerThread = new std::thread(&EventHandler::eventHandlerThread, this);
}

EventHandler::~EventHandler()
{

}

void EventHandler::setParent(Base* parent)
{
    m_base = parent;
}

void EventHandler::stop()
{
    m_MIsStopping.lock();
    m_isStopping = true;
    m_MIsStopping.unlock();
}

bool EventHandler::isStoping()
{
    m_MIsStopping.lock();
    bool toReturn = m_isStopping;
    m_MIsStopping.unlock();
    return m_isStopping;
}

void EventHandler::sendAnswer(std::shared_ptr<BaseEvent> event)
{
    for (int loop = 0; loop != m_userHandlers.size(); loop++)
    {
        m_userHandlers[loop]->receiveAnswer(event);
    }
}

void EventHandler::launch()
{
    std::cout<<"Debut du lancement des eventHandler : "<<std::endl;
    for (int loop = 0; loop != m_userHandlers.size(); loop++)
    {
        std::cout<<"Un eventHandler lance"<<std::endl;
        m_userHandlers[loop]->launch();
    }
}

void EventHandler::transmitEvent(std::shared_ptr<UserEvent> event)
{
    m_base->proceedEvent(event);
}

std::vector<std::weak_ptr<AiguillageHandler>> EventHandler::getAiguillageHandlers()
{
    return m_base->getAiguillageHandlers();
}


std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>> EventHandler::findAiguillageById(int aiguillageHandlerId,int aiguillageId, bool* success)
{
    return m_base->findAiguillageById(aiguillageHandlerId,aiguillageId, success);
}

std::weak_ptr<AiguillageHandler> EventHandler::getAiguillageHandlerById(int aiguillageHandlerId)
{
    return m_base->findAiguillageHandlerById(aiguillageHandlerId);
}

std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>> EventHandler::getAiguillage()
{
    return m_base->getAiguillages();
}

nlohmann::json EventHandler::save()
{
    nlohmann::json toReturn;
    toReturn["console_user_handler"] = m_userHandlers[1]->save();
    toReturn["network_user_handler"] = m_userHandlers[0]->save();
    return toReturn;
}

