#include "AiguillageHandler.h"
#include "AiguillageManager.h"
#include <iostream>
#include <memory>

const sf::Time AiguillageHandler::switchDuration (sf::seconds(0.1));

AiguillageHandler::~AiguillageHandler()
{
    //dtor
}

int AiguillageHandler::getId()
{
    return m_id;
}

bool AiguillageHandler::initializeId(int id)
{
    if (m_isInitialized == false)
    {
        m_id = id;
        m_isInitialized = true;
    }
}

std::string AiguillageHandler::getName()
{
    return m_nom;
}

bool AiguillageHandler::isSinglePinModeCompatible()
{
    return m_isSinglePinModeCompatible;
}

