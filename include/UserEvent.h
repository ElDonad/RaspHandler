#ifndef USEREVENT_H
#define USEREVENT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <BaseAiguillage.h>
#include "json.hpp"

class UserEvent
{
    public:
        enum UserEventTypes
        {
            AddAiguillage,
            RemoveAiguillage,
            Stop,
            ChangeAiguillageState,
            ConfirmPositions,
            AddAlimentation,
            RemoveAlimentation,
            RequestSave
        };

        struct AddAiguillageEvent
        {
            BaseAiguillage::AiguillageType type;
            std::string tag;
            BaseAiguillage::AiguillageParams* params;
            int idAiguillageHandler;
            nlohmann::json comp;
        };

        struct RemoveAiguillageEvent
        {
            std::string tag;
            int aiguillageHandlerId;
            nlohmann::json comp;
        };

        struct StopEvent
        {
            std::string raison;
        };

        struct ChangeAiguillageStateEvent
        {
            int aiguillageHandlerId, aiguillageId;
            BaseAiguillage::Direction targetState;
        };

        struct ConfirmPositionsEvent
        {

        };

        struct AddAlimentationEvent
        {
            std::string alimentationName;
            int idAiguillageHandler;
            int pinAlim;
            nlohmann::json comp;
        };

        struct RemoveAlimentationEvent
        {
            std::string alimentationName;
            int idAiguillageHandler;
            int idPinAlim;
            nlohmann::json comp;
        };

        struct RequestSaveEvent
        {

        };

        //union
        //{
            AddAiguillageEvent addAiguillageEvent;
            RemoveAiguillageEvent removeAiguillageEvent;
            StopEvent stopEvent;
            ChangeAiguillageStateEvent changeAiguillageStateEvent;
            ConfirmPositionsEvent confirmPositionsEvent;
            AddAlimentationEvent addAlimentationEvent;
            RemoveAlimentationEvent removeAlimentationEvent;
            RequestSaveEvent requestSaveEvent;
       // };

        UserEvent(UserEventTypes type, nlohmann::json sender = nlohmann::json::object());
        //UserEvent(UserEvent const& b); deprecated
        /** Default destructor */
        virtual ~UserEvent();

        UserEventTypes getType();
        int getId();
        nlohmann::json getSender();


    protected:
        UserEventTypes m_userEventType;
        int m_id;
        static int m_currentAvailableId;
        static int getNextAvailableId();
        nlohmann::json m_sender;
};

#endif // USEREVENT_H
