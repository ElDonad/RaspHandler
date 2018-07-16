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
            RemoveAlimentation
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

        //union
        //{
            AddAiguillageEvent addAiguillageEvent;
            RemoveAiguillageEvent removeAiguillageEvent;
            StopEvent stopEvent;
            ChangeAiguillageStateEvent changeAiguillageStateEvent;
            ConfirmPositionsEvent confirmPositionsEvent;
            AddAlimentationEvent addAlimentationEvent;
            RemoveAlimentationEvent removeAlimentationEvent;
       // };

        UserEvent(UserEventTypes type);
        //UserEvent(UserEvent const& b); deprecated
        /** Default destructor */
        virtual ~UserEvent();

        UserEventTypes getType();
        int getId();


    protected:
        UserEventTypes m_userEventType;
        int m_id;
        static int m_currentAvailableId;
        static int getNextAvailableId();
};

#endif // USEREVENT_H
