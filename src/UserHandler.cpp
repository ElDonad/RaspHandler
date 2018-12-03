#include "UserHandler.h"
#include "EventHandler.h"

UserHandler::UserHandler()
{
    //ctor
}

UserHandler::~UserHandler()
{
    //dtor
}

void UserHandler::receiveAnswer(std::shared_ptr<BaseEvent> answer)
{

}

bool UserHandler::isStopping()
{
    m_MisStopping.lock();
    bool toReturn = m_stop;
    m_MisStopping.unlock();
    return toReturn;
}

void UserHandler::stop()
{
    m_MisStopping.lock();
    m_stop = true;
    m_MisStopping.unlock();

}

