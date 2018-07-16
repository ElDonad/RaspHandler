#include "AiguillageManager.h"
#include "Base.h"

int AiguillageManager::m_nextAvailableInt = 1;
int AiguillageManager::m_nextBackupId = 1;
int AiguillageManager::m_nextAlimentationId = 1;

AiguillageManager::AiguillageManager(Base* base) : m_base(base)
{
    m_aiguillageHandlers.push_back(std::shared_ptr<AiguillageHandler>(new DirectAiguillageHandler(this)));
    m_aiguillageHandlers.back()->initializeId(this->getNextId());
    launch();
    //m_aiguillageHandlers.emplace(getNextId(), std::unique_ptr<AiguillageHandler> (new DirectAiguillageHandler(this)));
}

AiguillageManager::~AiguillageManager()
{
    //dtor
}

void AiguillageManager::proceedEvent(std::shared_ptr<UserEvent> event)
{
    switch (event->getType())
    {
    case UserEvent::UserEventTypes::AddAiguillage:
        {
            switch (event->addAiguillageEvent.type)
            {
            case BaseAiguillage::AiguillageType::SimpleAiguillage1:
                {
                    int aiguillageHandlerId = event->addAiguillageEvent.idAiguillageHandler;
                    for (auto it = m_aiguillageHandlers.begin(); it != m_aiguillageHandlers.end(); ++it)
                    {
                        std::cout<<(*it)->getId()<<std::endl;
                        if((*it)->getId() == aiguillageHandlerId)
                        {
                            //L'AiguillageHandler a été trouvé
                            (*it)->addAiguillage(std::shared_ptr<BaseAiguillage>(new SimpleAiguillage1(dynamic_cast<SimpleAiguillage1::AiguillageParams*>(event->addAiguillageEvent.params),*it)),event->addAiguillageEvent.comp);
                            return;

                        }
                    }
                    std::shared_ptr<BaseEvent> addAborted (new BaseEvent(BaseEvent::EventTypes::AddingAiguillageAborted, event->getId()));
                    addAborted->addingAiguillageAbortedEvent.errors.push_back("ID du handler invalide");
                    m_base->transmitEvent(addAborted);
                    break;

                }
// TODO (Elie#1#): Ajouter les autres types d'aiguillage ...

            }
            break;
        }

        case UserEvent::UserEventTypes::AddAlimentation:
        {
            int aiguillageHandlerId = event->addAlimentationEvent.idAiguillageHandler;
            std::shared_ptr<AiguillageHandler> aiguillageHandler = findAiguillageHandlerById(aiguillageHandlerId).lock();
            std::shared_ptr<SinglePinModeCompatibleAiguillageHandler> convertedAiguillageHandler = std::dynamic_pointer_cast <SinglePinModeCompatibleAiguillageHandler>(aiguillageHandler);
            convertedAiguillageHandler->addAlimentation(event->addAlimentationEvent.pinAlim, event->addAlimentationEvent.alimentationName, event->addAlimentationEvent.comp);
//            for (auto it = callback.begin(); it != callback.end(); ++it)
//            {
//                if ((*it)->getType() == AiguillageHandlerEvent::AiguillageHandlerEventTypes::AlimentationAdded)//si les callbacks sont bien appropriés
//                {
//                    if ((*it)->alimentationAddedEvent.done == true)//si tout s'est passé correctement
//                    {
//                        std::shared_ptr<BaseEvent> createdEvent (new BaseEvent(BaseEvent::EventTypes::AddingAlimentationConfirmed, event->getId()));
//                        createdEvent->addingAlimentationConfirmedEvent.aiguillageHandlerId = event->addAlimentationEvent.idAiguillageHandler;
//                        createdEvent->addingAlimentationConfirmedEvent.alimentationId = (*it)->alimentationAddedEvent.alimentationId;
//                        createdEvent->addingAlimentationConfirmedEvent.alimentationName = event->addAlimentationEvent.alimentationName;
//                        m_base->transmitEvent(createdEvent);
//
//                        std::shared_ptr<BaseEvent> creationEvent (new BaseEvent(BaseEvent::EventTypes::AlimentationAdded, 0));
//                        creationEvent->alimentationAddedEvent.aiguillageHanderId = event->addAlimentationEvent.idAiguillageHandler;
//                        creationEvent->alimentationAddedEvent.alimentationId = (*it)->alimentationAddedEvent.alimentationId;
//                        creationEvent->alimentationAddedEvent.alimentationName = event->addAlimentationEvent.alimentationName;
//                        m_base->transmitEvent(creationEvent);
//                    }
//                    else
//                    {
//                        std::shared_ptr<BaseEvent> abortedEvent (new BaseEvent(BaseEvent::EventTypes::AddingAlimentationAborted, event->getId()));
//                        abortedEvent->addingAlimentationAbortedEvent.aiguillageHandlerId = event->addAlimentationEvent.idAiguillageHandler;
//                        abortedEvent->addingAlimentationAbortedEvent.alimentationId =  (*it)->alimentationAddedEvent.alimentationId;
//                        abortedEvent->addingAlimentationAbortedEvent.alimentationName = event->addAlimentationEvent.alimentationName;
//                        switch((*it)->alimentationAddedEvent.errorType)
//                        {
//                        case AiguillageHandlerEvent::AlimentationAddedEvent::ErrorTypes::PinAlreadyUsed:
//                            {
//                                abortedEvent->addingAlimentationAbortedEvent.errors.push_back("Pin already used");
//                                break;
//                            }
//                        case AiguillageHandlerEvent::AlimentationAddedEvent::ErrorTypes::InvalidHandler:
//                            {
//                                abortedEvent->addingAlimentationAbortedEvent.errors.push_back("Invalid AiguillageHandler id");
//                                break;
//                            }
//                        case AiguillageHandlerEvent::AlimentationAddedEvent::ErrorTypes::Undefined:
//                            {
//                                abortedEvent->addingAlimentationAbortedEvent.errors.push_back("Undefined");
//                                break;
//                            }
//                        }
//                        m_base->transmitEvent(abortedEvent);
//                    }
//                }
//            }


            break;
        }

        case UserEvent::UserEventTypes::Stop:
        {
            this->stop();
        }
        case UserEvent::UserEventTypes::ChangeAiguillageState:
            {
                std::shared_ptr<AiguillageHandler> handler = findAiguillageHandlerById(event->changeAiguillageStateEvent.aiguillageHandlerId).lock();
                std::shared_ptr<BaseAiguillage> aiguillage = handler->findAiguillageById(event->changeAiguillageStateEvent.aiguillageId).second.lock();
                std::vector<std::shared_ptr<AiguillageHandlerEvent>> errors;//à implémenter la sécurité
                //aiguillage->changeSens(event->changeAiguillageStateEvent.targetState, errors);
                handler->switchAiguillage(event->changeAiguillageStateEvent.aiguillageId, event->changeAiguillageStateEvent.targetState);

// TODO (Elie#2#): Eventuellement lancer également le signal ChangingAiguillageStateAbortes/Confirmed (et gérer les erreurs aussi...)

                std::shared_ptr<BaseEvent> retourEvent (new BaseEvent(BaseEvent::EventTypes::AiguillageStateChanged,0));
                retourEvent->aiguillageStateChangedEvent.aiguillageHandlerId = handler->getId();
                retourEvent->aiguillageStateChangedEvent.aiguillageId = aiguillage->getId();
                retourEvent->aiguillageStateChangedEvent.direction = aiguillage->getTargetDirection();
                m_base->transmitEvent(retourEvent);

                break;
            }

        default:
            {
                std::cout<<"error"<<std::endl;
            }

    }
}

int AiguillageManager::getNextId()
{
    int id = m_nextAvailableInt;
    m_nextAvailableInt++;
    return id;
}

void AiguillageManager::launch()
{

}

//void AiguillageManager::proceedEvent(std::shared_ptr<AiguillageEvent> event)
//{
//    switch (event->getType())
//    {
//    case AiguillageEvent::AiguillageEventTypes::RegisterHandler:
//        {
//            if (event->registerHandlerEvent.hadReservedIdBefore == false)//nouveau handler
//            {
//                int id = getNextId();
//                m_aiguillageHandlers.push_back(std::unique_ptr<AiguillageHandler>(event->registerHandlerEvent.aiguillageHandlerToRegister));
//                m_aiguillageHandlers.back()->initializeId(getNextId());
//                event->registerHandlerEvent.aiguillageHandlerToRegister->launch();
//
//                std::shared_ptr<BaseEvent> newHandler (new BaseEvent(BaseEvent::EventTypes::AiguillageHandlerRegistered,0));
//                newHandler->aiguillageHandlerRegisteredEvent.aiguillageHandlerId = id;
//                newHandler->aiguillageHandlerRegisteredEvent.isReconnection = false;
//                m_base->transmitEvent(newHandler);
//            }
//            else if (event->registerHandlerEvent.hadReservedIdBefore == true)//handler de backup
//            {
//                int previousId = 0;
//                previousId = m_backupsIds[event->registerHandlerEvent.backupId];
//                if (previousId != 0)//si tout s'est bien passé
//                {
//                    m_aiguillageHandlers.push_back(std::unique_ptr<AiguillageHandler>(event->registerHandlerEvent.aiguillageHandlerToRegister));
//                    m_aiguillageHandlers.back()->initializeId(previousId);
//                    std::shared_ptr<BaseEvent> backupAiguillageHandler (new BaseEvent(BaseEvent::EventTypes::AiguillageHandlerRegistered, 0));
//                    backupAiguillageHandler->aiguillageHandlerRegisteredEvent.isReconnection = true;
//                    backupAiguillageHandler->aiguillageHandlerRegisteredEvent.aiguillageHandlerId = previousId;
//                    m_base->transmitEvent(backupAiguillageHandler);
//                }
//            }
//            break;
//        }
//
//        case AiguillageEvent::AiguillageEventTypes::UnregisterHandler:
//        {
//            m_backupsIds.emplace(event->unregisterHandlerEvent.backupId, event->unregisterHandlerEvent.aiguillageHandlerId);
//            for (int loop = 0; loop != m_aiguillageHandlers.size(); loop++)
//            {
//                if (m_aiguillageHandlers[loop]->getId() == event->unregisterHandlerEvent.aiguillageHandlerId)
//                {
//                    m_aiguillageHandlers.erase(m_aiguillageHandlers.begin() + loop);
//                }
//            }
//
//            std::shared_ptr<BaseEvent> truc (new BaseEvent(BaseEvent::EventTypes::AiguillageHandlerUnregistered,0));
//            truc->aiguillageHandlerUnregisteredEvent.aiguillageHandlerId = event->unregisterHandlerEvent.aiguillageHandlerId;
//            m_base->transmitEvent(truc);
//            break;
//        }
//
//        case AiguillageEvent::AiguillageEventTypes::RequestSinglePinAlim:
//        {
//            bool foundAiguillageHandler;
//            std::shared_ptr<SinglePinModeCompatibleAiguillageHandler> handler = std::dynamic_pointer_cast<SinglePinModeCompatibleAiguillageHandler>(findAiguillageHandlerById(event->requestSinglePinAlimEvent.aiguillageHandlerId, &foundAiguillageHandler).lock());
//            if (foundAiguillageHandler == true)
//            {
//                SinglePinModeCompatibleAiguillageHandler::AiguillageHandlerSwitchingAlimState result = handler->toggleAlim(event->requestSinglePinAlimEvent.alimId, event->requestSinglePinAlimEvent.alimState);
//                switch (result)
//                {
//                case SinglePinModeCompatibleAiguillageHandler::AiguillageHandlerSwitchingAlimState::AlimDone:
//                    {
//                        std::shared_ptr<BaseEvent> doneEvent (new BaseEvent(BaseEvent::EventTypes::AlimToggled,0));
//                        doneEvent->alimToggledEvent.aiguillageHandlerId = event->requestSinglePinAlimEvent.aiguillageHandlerId;
//                        doneEvent->alimToggledEvent.alimentationState = event->requestSinglePinAlimEvent.alimState;
//                        doneEvent->alimToggledEvent.alimentationToggledId = event->requestSinglePinAlimEvent.alimId;
//                        m_base->transmitEvent(doneEvent);
//                    }
//                }
//            }
//            else
//            {
//// TODO (Elie#5#): à finir
//
//            }
//
//
//            break;
//        }
//    }
//}

int AiguillageManager::getBackupId()
{
    int returnId = m_nextAvailableInt;
    m_nextAvailableInt++;
    return returnId;
}

int AiguillageManager::getAlimId()
{
    int returnId = m_nextAlimentationId;
    m_nextAlimentationId++;
    return returnId;
}

void AiguillageManager::stop()
{

}

std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>> AiguillageManager::getAiguillages()
{
    std::vector<std::pair<nlohmann::json,std::weak_ptr <BaseAiguillage>>> toReturn;
    for (auto it = m_aiguillageHandlers.begin(); it != m_aiguillageHandlers.end(); ++it)
    {
        auto aiguillagesList = (*it)->getAiguillages();
        for (auto it2 = aiguillagesList.begin(); it2 != aiguillagesList.end(); ++it2)
        {
            toReturn.push_back(std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>((it2->first),(it2->second)));
        }
    }
    return toReturn;
}

std::vector<std::weak_ptr<AiguillageHandler> > AiguillageManager::getAiguillageHandlers()
{
    std::vector<std::weak_ptr<AiguillageHandler> > toReturn;
    for (auto it = m_aiguillageHandlers.begin(); it != m_aiguillageHandlers.end(); ++it)
    {
        toReturn.emplace_back(*it);
    }
    return toReturn;
}

std::weak_ptr<AiguillageHandler> AiguillageManager::findAiguillageHandlerById(int id, bool* success)
{
    for (auto it = m_aiguillageHandlers.begin(); it != m_aiguillageHandlers.end(); ++it)
    {
        if ((*it)->getId() == id)
        {
            if (success != nullptr)
                *success = true;
            return std::weak_ptr<AiguillageHandler>(*it);
        }
    }
}

void AiguillageManager::proceedEvent(std::shared_ptr<AiguillageHandlerEvent> event)
{
    switch(event->getType())
    {
    case AiguillageHandlerEvent::AiguillageHandlerEventTypes::AiguillageAdded:
        {
            std::shared_ptr<BaseEvent> r_event;
            if (event->aiguillageAddedEvent.done == true)//réussi
            {
                std::shared_ptr<BaseEvent> newEvent (new BaseEvent(BaseEvent::EventTypes::AiguillageAdded,0));
                newEvent->aiguillageAddedEvent.aiguillageId = event->aiguillageAddedEvent.aiguillageId;
                newEvent->aiguillageAddedEvent.handlerId = event->aiguillageAddedEvent.aiguillageHandlerId;
                newEvent->aiguillageAddedEvent.type = findAiguillageHandlerById(event->aiguillageAddedEvent.aiguillageHandlerId).lock()->findAiguillageById(event->aiguillageAddedEvent.aiguillageId).second.lock()->getType();

                r_event = std::shared_ptr<BaseEvent>(new BaseEvent(BaseEvent::EventTypes::AddingAiguillageConfirmed,event->getCallbackId()));
                r_event->addingAiguillageConfirmedEvent.aiguillageHandlerId = event->aiguillageAddedEvent.aiguillageHandlerId;
                r_event->addingAiguillageConfirmedEvent.aiguillageId = event->aiguillageAddedEvent.aiguillageId;
                m_base->transmitEvent(r_event);
            }
            else
            {
                std::shared_ptr<BaseEvent> abortedEvent (new BaseEvent(BaseEvent::EventTypes::AddingAiguillageAborted,event->getCallbackId()));
                for(auto it = event->aiguillageAddedEvent.errors.begin(); it != event->aiguillageAddedEvent.errors.end(); ++it)
                {
                    abortedEvent->addingAiguillageAbortedEvent.errors.push_back(*it);
                }
                m_base->transmitEvent(abortedEvent);
            }
            break;

        }
        case AiguillageHandlerEvent::AiguillageDeleted:
            {
                if (event->aiguillageDeletedEvent.done == true)
                {
                    std::shared_ptr<BaseEvent> r_event (new BaseEvent(BaseEvent::EventTypes::RemovingAiguillageConfirmed,event->getCallbackId()));
                    r_event->removingAiguillageConfirmedEvent.tag = event->aiguillageDeletedEvent.aiguillageDeleted;
                    r_event->removingAiguillageConfirmedEvent.aiguillageHandlerId = event->aiguillageDeletedEvent.aiguillageHandlerId;
                    r_event->removingAiguillageConfirmedEvent.aiguillageId = event->aiguillageDeletedEvent.aiguillageDeletedId;
                    m_base->transmitEvent(r_event);

                    std::shared_ptr<BaseEvent> r_event2 (new BaseEvent(BaseEvent::EventTypes::AiguillageRemoved,0));
                    r_event2->aiguillageRemovedEvent.aiguillageHandlerId = event->aiguillageDeletedEvent.aiguillageHandlerId;
                    r_event2->aiguillageRemovedEvent.aiguillageId = event->aiguillageDeletedEvent.aiguillageDeletedId;
                    r_event2->aiguillageRemovedEvent.tag = event->aiguillageDeletedEvent.aiguillageDeleted;
                    m_base->transmitEvent(r_event2);
                }
                else
                {
// TODO (Elie#5#): à finir ...
//

                }
                break;
            }
        case AiguillageHandlerEvent::AiguillageHandlerEventTypes::AlimentationAdded:
            {
                std::cout<<"Signal provisoire : alimentation ajoutée (depuis l'aiguillageManager)"<<std::endl;
                break;
            }
        case AiguillageHandlerEvent::AiguillageHandlerEventTypes::AlimentationDeleted:
            {
                std::cout<<"Signal provisoire : alimentation supprimée (depuis l'aiguillageManager)"<<std::endl;
                break;
            }
        case AiguillageHandlerEvent::AiguillageHandlerEventTypes::RequestSinglePinAlim:
            {
                std::shared_ptr <SinglePinModeCompatibleAiguillageHandler> handler = std::dynamic_pointer_cast<SinglePinModeCompatibleAiguillageHandler>(findAiguillageHandlerById(event->requestSinglePinAlimEvent.aiguillageHandlerId).lock());
                handler->toggleAlim(event->requestSinglePinAlimEvent.alimId, event->requestSinglePinAlimEvent.state);
                break;

            }
    }
}

std::pair<nlohmann::json,std::shared_ptr<BaseAiguillage>> AiguillageManager::findAiguillageById(int aiguillageHandlerId, int aiguillageId, bool* success)
{
    std::shared_ptr<AiguillageHandler> handler = findAiguillageHandlerById(aiguillageHandlerId, success).lock();
    if (success == nullptr || *success)
    {
        auto toReturn = handler->findAiguillageById(aiguillageId,success);
        return std::pair<nlohmann::json,std::shared_ptr<BaseAiguillage>>(toReturn.first,toReturn.second.lock());
    }
    else
    {
        return std::pair<nlohmann::json,std::shared_ptr<BaseAiguillage>> ();
    }
}

