#include "AiguillageHandlerEvent.h"

AiguillageHandlerEvent::AiguillageHandlerEvent(AiguillageHandlerEventTypes type, int callback)
{
    m_type = type;
}

AiguillageHandlerEvent::~AiguillageHandlerEvent()
{
    //dtor
}

int AiguillageHandlerEvent::getCallbackId()
{
    return m_idOfRequest;
}

AiguillageHandlerEvent::AiguillageHandlerEventTypes AiguillageHandlerEvent::getType()
{
    return m_type;
}
