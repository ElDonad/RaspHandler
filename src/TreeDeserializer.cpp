#include "TreeDeserializer.h"

TreeDeserializer::TreeDeserializer(nlohmann::json globalJSon)
{
    m_globalJSON = globalJSon;
    m_currentPos = &m_globalJSON;
}

TreeDeserializer::~TreeDeserializer()
{
    //dtor
}
