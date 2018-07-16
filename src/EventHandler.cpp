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
    m_handlerThread = new sf::Thread(EventHandler::eventHandlerThread, this);
    m_userHandlers.push_back(dynamic_cast<UserHandler*>(new NetworkUserHandler(this)));
    m_userHandlers.push_back(dynamic_cast<UserHandler*>(new ConsoleUserHandler(this)));

}

EventHandler::~EventHandler()
{
    //dtor
}

void EventHandler::setParent(Base* parent)
{
    m_base = parent;
}

void EventHandler::eventHandlerThread()
{
    int a;
    a++;
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
    m_handlerThread->launch();
    for (int loop = 0; loop != m_userHandlers.size(); loop++)
    {
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

