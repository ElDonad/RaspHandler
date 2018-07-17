#include "SimpleAiguillage1.h"

SimpleAiguillage1::SimpleAiguillage1(std::string name,int pin,std::weak_ptr<AiguillageHandler> parent,PinState defaultLeftPinState) : SimpleAiguillage({Direction::Left,Direction::Right})
{
    m_tag = name;
    m_directionsMap = {
        {Direction::Left, SinglePinModePin(pin,defaultLeftPinState)},
        {Direction::Right, SinglePinModePin(pin, getInvertedPinState(defaultLeftPinState))}
    };

    m_type = BaseAiguillage::AiguillageType::SimpleAiguillage1;
    m_mode = BaseAiguillage::AiguillageMode::Simple;
    //thread
    m_isDisponible = true;
    m_id = getNextDisponibleId();
}

SimpleAiguillage1::SimpleAiguillage1(SimpleAiguillage1::AiguillageParams* params, std::weak_ptr<AiguillageHandler> parent) : SimpleAiguillage(std::vector<Direction>({Direction::Left,Direction::Right}))
{
    m_type = AiguillageType::SimpleAiguillage1;
    SimpleAiguillage1::AiguillageParams* definitiveParams = dynamic_cast<SimpleAiguillage1::AiguillageParams*>(params);
    m_tag = definitiveParams->name;
    int pin = definitiveParams->leftDirectionPin.pinNumber;
    m_directionsMap = std::map<BaseAiguillage::Direction, SimpleAiguillage::SinglePinModePin>{
        {Direction::Left, definitiveParams->leftDirectionPin},
        {Direction::Right, definitiveParams->rightDirectionPin}
    };
    m_mode = BaseAiguillage::AiguillageMode::Simple;
    m_parent = parent;
//    m_alimId = params->alimId;
    m_targetDirection = params->direction;

    m_isDisponible = true;
    m_isStopping = false;
    m_alimentationId = params->alimId;
    m_alimentationHandlerId = params->alimHandlerId;


    delete params;
    m_id = getNextDisponibleId();
}

//SimpleAiguillage1::~SimpleAiguillage1()
//{
//    ~SimpleAiguillage();
//}

bool SimpleAiguillage1::changeSens(Direction newDirection, std::vector<ErrorsAiguillage>& errors)
{
    if (newDirection == m_validDirections[0] || newDirection == m_validDirections[1])
    {
        if (newDirection != m_targetDirection)
        {
             if (m_isDisponible == true)
             {
                m_targetDirection = newDirection;
                return true;
             }
             else if (m_isDisponible == false)
             {
                 errors.push_back(ErrorsAiguillage::Indisponible);
                 return false;
             }
        }

        else
        {
            errors.push_back(ErrorsAiguillage::InvalidDirection);
            return false;
        }
    }

}

bool SimpleAiguillage1::changeSens(int direction, std::vector<ErrorsAiguillage>& errors)
{
    Direction finalDirection = convertIntIntoDirection(direction);
    return changeSens(finalDirection, errors);
}

SimpleAiguillage1::Direction SimpleAiguillage1::getCurrentDirection()
{
    return m_currentDirection;
}

SimpleAiguillage1::SinglePinModePin SimpleAiguillage1::getPin(Direction direction)
{
    return m_directionsMap[direction];
}

void SimpleAiguillage1::directionChanger()
{
    return SimpleAiguillage::directionChanger();
}

std::vector <int> SimpleAiguillage1::getUsedPins()
{
    std::vector<int> usedPins;
    usedPins.push_back(m_directionsMap[Direction::Left].pinNumber);
    return usedPins;
}

SimpleAiguillage1::Direction SimpleAiguillage1::getTargetDirection()
{
    return m_targetDirection;
}




