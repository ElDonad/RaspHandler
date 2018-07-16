#ifndef BASEEVENT_H
#define BASEEVENT_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "BaseAiguillage.h"
#include "json.hpp"

class Alimentation;
class BaseEvent
{
    public:

        enum EventTypes
        {
            Confirmed,
            AiguillageAdded,
            AiguillageRemoved,
            //Pour le client qui crée ou supprime un aiguillage :
            AddingAiguillageConfirmed,
            RemovingAiguillageConfirmed,
            AddingAiguillageAborted,
            RemovingAiguillageAborted,
            Stopping,
            AiguillageStateChanged,
            ChangingAiguillageStateAborted,
            ChangingAiguillageStateConfirmed,
            AiguillageHandlerUnregistered,//!< Appelé quand un AiguillageHandler est indisponible provisoirement ou définitivement.
            AiguillageHandlerRegistered,//!< Est appelé quand un AiguillageHandler apparaît OU est reconnecté, auquel cas la valeur membre isReconnection permet de s'assurer qu'il s'agit bien d'une reconnection et pas d'un nouveau AiguillageHandler avec le même nom.
            //Alimentations
            AlimentationAdded,
            AlimentationDeleted,
            AddingAlimentationConfirmed,
            RemovingAlimentationConfirmed,
            AddingAlimentationAborted,
            RemovingAlimentationAborted,
            AlimToggled
        };

        struct AlimToggledEvent
        {
            int alimentationToggledId, aiguillageHandlerId;
            bool alimentationState;
        };

        struct ChangingAiguillageStateAbortedEvent
        {
            std::string tag;
            std::vector<BaseAiguillage::ErrorsAiguillage> errors;
        };

        struct ChangingAiguillageStateConfirmedEvent
        {
            std::string tag;
            int newDirection;
        };

        struct ConfirmedEvent
        {

        };

        struct AiguillageAddedEvent
        {
            BaseAiguillage::AiguillageType type;
            int handlerId, aiguillageId;
        };

        struct AiguillageRemovedEvent
        {
            std::string tag;
            int aiguillageId, aiguillageHandlerId;
        };

        struct AddingAiguillageConfirmedEvent
        {
            int aiguillageId, aiguillageHandlerId;
        };

        struct RemovingAiguillageConfirmedEvent
        {
            int aiguillageHandlerId, aiguillageId;//!<Ne pourront plus être utilisés pour récupérer les infos à propos de l'aiguillage.
            std::string tag;
        };

        struct StoppingEvent
        {
            std::string raison;
        };

        struct AiguillageStateChangedEvent
        {
            int aiguillageHandlerId, aiguillageId;
            nlohmann::json comp;
            BaseAiguillage::Direction direction;
        };

        struct AddingAiguillageAbortedEvent
        {
            std::vector <std::string> errors;
        };

        struct RemovingAiguillageAbortedEvent
        {
            std::vector <std::string> errors;
        };

        struct AiguillageHandlerUnregisteredEvent
        {
            int aiguillageHandlerId;
        };

        struct AiguillageHandlerRegisteredEvent
        {
            int aiguillageHandlerId;
            bool isReconnection;
        };

        struct AlimentationAddedEvent
        {
            std::string alimentationName;
            int aiguillageHanderId, alimentationId;
        };

        struct AlimentationDeletedEvent
        {
            int aiguillageHandlerId, alimentationId;
        };

        struct AddingAlimentationConfirmedEvent
        {
            std::string alimentationName;
            int aiguillageHandlerId, alimentationId;
        };

        struct RemovingAlimentationConfirmedEvent
        {
            std::string alimentationName;
            int aiguillageHandlerId, alimentationId;
        };

        struct AddingAlimentationAbortedEvent
        {
            std::vector<std::string> errors;
            std::string alimentationName;
            int aiguillageHandlerId, alimentationId;
        };

        struct RemovingAlimentationAbortedEvent
        {
            std::vector<std::string> errors;
            std::string alimentationName;
            int aiguillageHandlerId, alimentationId;
        };

        BaseEvent(EventTypes type, int id, std::string param1String);
        BaseEvent(EventTypes type, int id);
        BaseEvent(EventTypes type, int id, std::string param1String, int param2Int);
        BaseEvent(EventTypes type, int id, std::vector<std::string> params);/**<  le constructeur utilisé par défaut pour TOUS les autres types de paramètres*/

        EventTypes getType();
        int getId();

        /** Default destructor */
        virtual ~BaseEvent();

        //union
        //{
            ConfirmedEvent confirmedEvent;
            AiguillageAddedEvent aiguillageAddedEvent;
            AiguillageRemovedEvent aiguillageRemovedEvent;
            AddingAiguillageConfirmedEvent addingAiguillageConfirmedEvent;
            RemovingAiguillageConfirmedEvent removingAiguillageConfirmedEvent;
            StoppingEvent stoppingEvent;
            AiguillageStateChangedEvent aiguillageStateChangedEvent;
            RemovingAiguillageAbortedEvent removingAiguillageAbortedEvent;
            AddingAiguillageAbortedEvent addingAiguillageAbortedEvent;
            ChangingAiguillageStateAbortedEvent changingAiguillageStateAbortedEvent;
            ChangingAiguillageStateConfirmedEvent changingAiguillageStateConfirmedEvent;
            AiguillageHandlerUnregisteredEvent aiguillageHandlerUnregisteredEvent;
            AiguillageHandlerRegisteredEvent aiguillageHandlerRegisteredEvent;
            AlimToggledEvent alimToggledEvent;
            AlimentationAddedEvent alimentationAddedEvent;
            AlimentationDeletedEvent alimentationDeletedEvent;
            AddingAlimentationConfirmedEvent addingAlimentationConfirmedEvent;
            RemovingAlimentationConfirmedEvent removingAlimentationConfirmedEvent;
            AddingAlimentationAbortedEvent addingAlimentationAbortedEvent;
            RemovingAlimentationAbortedEvent removingAlimentationAbortedEvent;
        //};

    protected:
        EventTypes m_type;
        int m_eventId;

};

#endif // BASEEVENT_H
