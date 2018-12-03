#ifndef TREESERIALIZER_H
#define TREESERIALIZER_H

#include "json.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <functional>

template <typename T>
class TreeSerializer
{
    public:
        TreeSerializer(T* toSerialize);
        virtual ~TreeSerializer();

        /** Cette fonction enregistre une fonction de la classe pour être retournée ensuite.
        @param name : Le nom de la classe EN COURS. Par la suite, pour retrouver ses données correspondantes, la classe concernée devra indiquer le chemin DEPUIS LA CLASSE ROOT.


        */
        void registerJSonNode(std::function<nlohmann::json(T*)> func,std::string name = "");
        nlohmann::json getTotalTree();

    private:
        struct serializeFunction
        {
            std::string name;
            std::function<nlohmann::json(T*)> func;
        };
        std::vector<serializeFunction> m_treeFunctions;
        T* m_toSerialize;
};

template <typename T>
TreeSerializer<T>::TreeSerializer(T* toSerialize)
{
    m_toSerialize = toSerialize;
}

template <typename T>
TreeSerializer<T>::~TreeSerializer()
{
    //dtor
}

//template<typename T>
//void TreeSerializer<T>::registerJSonNode(nlohmann::json (*saveFunction)(T*),std::string parentName)
//{
//    m_treeFunctions.push_back(serializeFunction(saveFunction,parentName));
//    std::cout<<"Fonction ajoutée : "<<saveFunction<<std::endl;
//    std::cout<<"Taille du treeSerializer : "<<m_treeFunctions.size()<<" noeuds."<<std::endl;
//}

template <typename T>
void TreeSerializer<T>::registerJSonNode(std::function <nlohmann::json(T*)> func, std::string parentName)
{
    serializeFunction truc;
    truc.func = func;
    truc.name = parentName;
    m_treeFunctions.push_back(truc);
}

template <typename T>
nlohmann::json TreeSerializer<T>::getTotalTree()
{
    nlohmann::json tree;
    nlohmann::json* tree_iterator = &tree;
    for (auto it = m_treeFunctions.begin(); it != m_treeFunctions.end(); ++it)
    {
        std::cout<<it->name<<std::endl;
        (*tree_iterator) = it->func(m_toSerialize);
        if (it + 1!= m_treeFunctions.end())
        {
            tree_iterator->operator[]((it + 1)->name) = nlohmann::json();
            tree_iterator = &tree_iterator->operator[]((it + 1)->name);
        }



    }
    return tree;
}

#endif // TREESERIALIZER_H
