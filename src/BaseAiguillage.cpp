#include "BaseAiguillage.h"
#include "AiguillageHandler.h"

BaseAiguillage::BaseAiguillage(std::vector<Direction>validDirections) : m_validDirections(validDirections), m_serializer(this), m_classicSerializer(this)
{
    registerJSONNode();
}

BaseAiguillage::BaseAiguillage(std::vector<Direction> validDirections, nlohmann::json savedData) : m_validDirections(validDirections), m_serializer(this), m_classicSerializer(this)
{
    m_id = savedData["id"];
    m_mode = savedData["mode"];
    m_type = savedData["type"];
    m_currentDirection = savedData["current_direction"];
    m_targetDirection = savedData["target_direction"];
    m_tag = savedData["tag"];
    registerJSONNode();
}
void BaseAiguillage::registerJSONNode()
{
    m_serializer.registerJSonNode([=](BaseAiguillage* toSerialize)
    {
        nlohmann::json toReturn;
        toReturn["id"] = toSerialize->m_id;
        toReturn["mode"] = toSerialize->m_mode;
        toReturn["type"] = toSerialize->m_type;
        toReturn["current_direction"] = toSerialize->m_currentDirection;
        toReturn["target_direction"] = toSerialize->m_targetDirection;
        toReturn["tag"] = toSerialize->m_tag;

        return toReturn;

    }, "BaseAiguillage");

    m_classicSerializer.registerJSonNode([=](BaseAiguillage* toSerialize)
    {
        nlohmann::json toReturn;
        toReturn["type"] = toSerialize->getStringifiedType();
        toReturn["id"] = toSerialize->m_id;
        toReturn["current_direction"] = toSerialize->m_currentDirection;
        toReturn["target_direction"] = toSerialize->m_targetDirection;
        toReturn["tag"] = toSerialize->m_tag;

        return toReturn;
    });
}

BaseAiguillage::AiguillageType BaseAiguillage::getType()
{
    return m_type;
}
BaseAiguillage::~BaseAiguillage()
{
    m_isStopping = true;
}

int BaseAiguillage::convertDirectionIntoInt(BaseAiguillage::Direction direction)
{
    switch (direction)
    {
    case Direction::Left:
        return 0;

    case Direction::Middle:
        return 2;

    case Direction::Right:
        return 1;

    default:
        return -1;
    }
}

BaseAiguillage::Direction BaseAiguillage::convertIntIntoDirection(int direction)
{
    switch (direction)
    {
    case 0:
        return Direction::Left;

    case 1:
        return Direction::Right;

    case 2:
        return Direction::Middle;
    default:
        return Direction::Invalid;
    }
}

std::string BaseAiguillage::getTag()
{
    return m_tag;
}

std::string BaseAiguillage::convertDirectionIntoString(Direction direction)
{
    switch (direction)
    {
    case Direction::Left:
        return "gauche";

    case Direction::Right:
        return "droite";

    case Direction::Invalid:
        return "invalide";
    default:
        return "inconnue";
    }
}

BaseAiguillage::Direction BaseAiguillage::convertStringIntoDirection(std::string direction)
{
    if (direction == "gauche")
        return Direction::Left;
    else if (direction == "droite")
        return Direction::Right;
    else
        return Direction::Invalid;
}

BaseAiguillage::AiguillageMode BaseAiguillage::getMode()
{
    return m_mode;
}

int BaseAiguillage::getId()
{
    return m_id;
}

std::weak_ptr<AiguillageHandler> BaseAiguillage::getParentAiguillageHandler()
{
    return m_parent;
}

void BaseAiguillage::setIndisponible(BaseAiguillage::IndisponibilityReasons reason)
{
    std::vector<IndisponibilityReasons> toReturn;
    toReturn.push_back(reason);
    setIndisponible(toReturn);
}

void BaseAiguillage::setIndisponible(std::vector<BaseAiguillage::IndisponibilityReasons> reasons)
{
    for (auto it = reasons.begin(); it != reasons.end(); ++it)
    {
        for (auto it2 = m_indisponibilityReasons.begin(); it2 != m_indisponibilityReasons.end(); ++it)
        {
            if (*it == *it2)//si déjà présent
                continue;
            else
            {
                m_indisponibilityReasons.push_back(*it);
                m_isDisponible = false;
            }
        }
    }
}

bool BaseAiguillage::setDisponible(BaseAiguillage::IndisponibilityReasons reason)
{
    for (auto it = m_indisponibilityReasons.begin(); it != m_indisponibilityReasons.end(); ++it)
    {
        if (*it == reason)
        {
            m_indisponibilityReasons.erase(it);
        }
    }
    if (m_indisponibilityReasons.size() == 0)//si aiguillage disponible
    {
        confirm();
        m_isDisponible = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool BaseAiguillage::isDisponible()
{
    return m_isDisponible;
}

std::vector<BaseAiguillage::Direction> BaseAiguillage::getValidDirections()
{
    return m_validDirections;
}

//std::shared_ptr<nlohmann::json> BaseAiguillage::constructSpecificAspect(std::shared_ptr<nlohmann::json> toConstruct)
//{
//    nlohmann::json added = *toConstruct;
//    m_id = added["id"];
//    m_mode = added["mode"];
//    m_type = added["type"];
//    m_currentDirection = added["current_direction"];
//    m_targetDirection = added["target_direction"];
//    m_tag = added["tag"];
//    return std::make_shared<nlohmann::json>(*toConstruct);
//
//}

void BaseAiguillage::mergeJSon(std::shared_ptr<nlohmann::json> main, nlohmann::json& toMerge)
{
    for (auto it = toMerge.begin(); it != toMerge.end(); ++it)
    {
        main->emplace(it.key(), it.value());
    }
}

nlohmann::json BaseAiguillage::save()
{
    return m_serializer.getTotalTree();
}

std::string BaseAiguillage::getStringifiedType()
{
    return "BaseAiguillage";
}

nlohmann::json BaseAiguillage::getClassicSerializedData()
{
    return m_classicSerializer.getTotalTree();
}

nlohmann::json BaseAiguillage::getCompInformations()
{
    return m_complementaryInformations;
}

void BaseAiguillage::setCompInformation(nlohmann::json newInfo, std::string identifier)
{
    m_complementaryInformations[identifier] = newInfo;
}
