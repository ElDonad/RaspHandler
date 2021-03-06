#include "Base.h"
#include "UserEvent.h"

using namespace std;

Base::Base(std::string savePath) : m_eventHandler(this), m_aiguillageManager(this)
{
    stop = false;
    m_eventHandler.setParent(this);
    m_savePath = savePath;
}

Base::Base(std::string savePath, nlohmann::json saveData) : m_eventHandler(this, saveData["event_handler"]), m_aiguillageManager(this, saveData["aiguillage_manager"])
{
    stop = false;
    m_eventHandler.setParent(this);
    m_savePath = savePath;
}

Base::~Base()
{
    std::cout<<"Base destroyed"<<endl;
}

void Base::proceedEvent(std::shared_ptr<UserEvent> event)
{
    switch (event->getType())
    {
    case UserEvent::AddAiguillage:
        {
            m_aiguillageManager.proceedEvent(event);
            break;
        }

    case UserEvent::ChangeAiguillageState:
        {
            m_aiguillageManager.proceedEvent(event);
            break;
        }

    case UserEvent::ConfirmPositions:
        {
            m_aiguillageManager.proceedEvent(event);
            break;
        }

    case UserEvent::RemoveAiguillage:
        {
            m_aiguillageManager.proceedEvent(event);
            break;
        }

    case UserEvent::Stop:
        {
            m_aiguillageManager.stop();
            m_eventHandler.stop();
            this->stop = true;
        }
    case UserEvent::AddAlimentation:
        {
            m_aiguillageManager.proceedEvent(event);
            break;
        }

    case UserEvent::RequestSave:
        {
            nlohmann::json saveData = this->save();
            ofstream file(m_savePath, ios::out | ios::trunc);
            file<<saveData.dump();
            break;
        }
    }
}

void Base::launch()
{
    m_aiguillageManager.launch();
    m_eventHandler.launch();
    while (stop == false)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void Base::transmitEvent(std::shared_ptr<BaseEvent> toTransmit)
{
    m_eventHandler.sendAnswer(toTransmit);
}

std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>> Base::getAiguillages()
{
    return m_aiguillageManager.getAiguillages();
}

std::vector<std::weak_ptr<AiguillageHandler>> Base::getAiguillageHandlers()
{
    return m_aiguillageManager.getAiguillageHandlers();
}
std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>> Base::findAiguillageById(int aiguillageHandlerId,int aiguillageId, bool* success)
{
    return m_aiguillageManager.findAiguillageById(aiguillageHandlerId,aiguillageId, success);
}
std::weak_ptr<AiguillageHandler> Base::findAiguillageHandlerById(int aiguillageHandlerId)
{
    return m_aiguillageManager.findAiguillageHandlerById(aiguillageHandlerId);
}

nlohmann::json Base::save()
{
    nlohmann::json toSave;
    toSave["aiguillage_manager"] = m_aiguillageManager.save();
    toSave["event_handler"] = m_eventHandler.save();
    return toSave;
}


