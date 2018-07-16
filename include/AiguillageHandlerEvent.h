#ifndef AIGUILLAGEHANDLEREVENT_H
#define AIGUILLAGEHANDLEREVENT_H
#include "BaseAiguillage.h"
#include "SimpleAiguillage.h"

/** Les events envoyés par le AiguillageHandler.

Il faut les distinguer des AiguillageEvent qui proviennent des aiguillages et dont la portée ne dépasse pas l'aiguillageHandler.


*/
class Alimentation;
class AiguillageHandlerEvent
{
    public:
        enum AiguillageHandlerEventTypes
        {
            AiguillageAdded,
            AiguillageDeleted,
            IndisponibleAiguillage,//!< Utile notamment pour les aiguillages par network. Signale qu'un aiguillage ne peut pas être utilisé.
            DisponibleAiguillage,
            Alert,//!<Pour envoyer un message à l'utilisateur.
            AlimentationAdded,
            AlimentationDeleted,

            RequestSinglePinAlim//!<Utilisé pour ordonner l'activation ou pas de l'alimentations
        };

        /** Default constructor */
        AiguillageHandlerEvent(AiguillageHandlerEventTypes type, int callback = 0);//!< @param callback : fourni éventuellement, sert en cas de réponse à un event levé par l'AiguillageManager.
        /** Default destructor */
        virtual ~AiguillageHandlerEvent();

        enum AlertTypes
        {
            Debug,
            Warning,
            Critical,
            Fatal
        };

        struct AiguillageAddedEvent
        {
            enum ErrorTypes
            {
                PinAlreadyUsed,
                NonCompatible,
                TagAlreadyUsed,
                Undefined,
                PinInvalid
            };
            int aiguillageHandlerId, aiguillageId;
            bool done = false;//false : see errorType
            ErrorTypes errorType;
            std::vector<std::string> errors;
        };

        struct AiguillageDeletedEvent
        {
            enum ErrorTypes
            {
                AiguillageNotFound,
                Undefined
            };
            std::string aiguillageDeleted;
            int aiguillageDeletedId, aiguillageHandlerId;
            bool done = false;
            ErrorTypes errorType;
            std::vector<std::string> errors;
        };

        struct AlertEvent
        {
            AlertTypes alertType;
            std::string message;
        };

        struct AlimentationAddedEvent
        {
            enum ErrorTypes
            {
                PinAlreadyUsed,
                InvalidHandler,
                Undefined

            };
            ErrorTypes errorType=Undefined;
            int aiguillageHandlerId, alimentationId;
            bool done = false;

        };

        struct AlimentationDeletedEvent
        {
            enum ErrorTypes
            {
                AlimentationNotFound,
                Undefined
            };
            ErrorTypes errorType=Undefined;
            int aiguillageHandlerId, alimentationId;
            bool done = false;


        };

        struct RequestSinglePinAlimEvent
        {
            int aiguillageHandlerId;
            int alimId;
            bool state;
        };
        AiguillageAddedEvent aiguillageAddedEvent;
        AiguillageDeletedEvent aiguillageDeletedEvent;
        AlertEvent alertevent;
        AlimentationAddedEvent alimentationAddedEvent;
        AlimentationDeletedEvent alimentationDeletedEvent;
        RequestSinglePinAlimEvent requestSinglePinAlimEvent;

        int getCallbackId();
        AiguillageHandlerEventTypes getType();

    protected:
        AiguillageHandlerEventTypes m_type;
        int m_idOfRequest;//si besoin
    private:
};

#endif // AIGUILLAGEHANDLEREVENT_H
