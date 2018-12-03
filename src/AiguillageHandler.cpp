#include "AiguillageHandler.h"
#include "AiguillageManager.h"
#include <iostream>
#include <memory>

const std::chrono::milliseconds AiguillageHandler::switchDuration (100);

AiguillageHandler::AiguillageHandler(AiguillageManager* parent) : m_serializer(this)
{
    m_isInitialized = false;
    registerJSONNode();
}

void AiguillageHandler::backup(nlohmann::json data)
{
    std::cout<<"Back up done"<<std::endl;
    m_id = data["id"];
    m_nom = data["nom"];
    m_isInitialized = true;
    m_isSinglePinModeCompatible = data["is_single_pin_mode_compatible"];
    registerJSONNode();

}

void AiguillageHandler::registerJSONNode()
{
    m_serializer.registerJSonNode([=](AiguillageHandler* toSerialize)
    {
        nlohmann::json toReturn;
        toReturn["id"] = toSerialize->m_id;
        toReturn["nom"] = toSerialize->m_nom;
        toReturn["is_single_pin_mode_compatible"] = toSerialize->m_isSinglePinModeCompatible;
        return toReturn;
    });
}

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

nlohmann::json AiguillageHandler::save()
{
    return m_serializer.getTotalTree();
}
