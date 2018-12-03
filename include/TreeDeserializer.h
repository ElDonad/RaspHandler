#ifndef TREEDESERIALIZER_H
#define TREEDESERIALIZER_H

#include <iostream>
#include <string>
#include <memory>
#include "json.hpp"

/** Système de restauration d'arborescence d'héritage automatique a partir d'un JSON.

Ce systeme fonctionne de manière simple : Lors de sa construction, le JSON global est passé en constructeur. Puis, chaque
membre de l'arbre requiert la récupération de sa version du JSON en fournissant son identifiant. Une fois l'identifiant reçu et le fragment envoyé, le système se place dans le
fragment passé pour envoyer les données au membre suivant.
*/
class TreeDeserializer
{
    public:
        TreeDeserializer(nlohmann::json globalJSON);
        virtual ~TreeDeserializer();
        nlohmann::json getSaveJSON(std::string id);

    protected:
        nlohmann::json m_globalJSON;
        nlohmann::json* m_currentPos;

    private:
};

#endif // TREEDESERIALIZER_H
