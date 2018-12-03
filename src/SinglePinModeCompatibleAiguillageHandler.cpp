#include "SinglePinModeCompatibleAiguillageHandler.h"

SinglePinModeCompatibleAiguillageHandler::SinglePinModeCompatibleAiguillageHandler(AiguillageManager* parent) : AiguillageHandler(parent)
{
    registerJSONNode();
}

void SinglePinModeCompatibleAiguillageHandler::backup(nlohmann::json backup)
{
    AiguillageHandler::backup(backup);
}

void SinglePinModeCompatibleAiguillageHandler::registerJSONNode()
{
    m_serializer.registerJSonNode([=](AiguillageHandler* toSerialize)
    {
        return nlohmann::json();
    }, "SinglePinModeCompatibleAiguillageHandler");
}
