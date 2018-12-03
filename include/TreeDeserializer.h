#ifndef TREEDESERIALIZER_H
#define TREEDESERIALIZER_H

#include <iostream>
#include <string>
#include <memory>
#include "json.hpp"

/** Syst�me de restauration d'arborescence d'h�ritage automatique a partir d'un JSON.

Ce systeme fonctionne de mani�re simple : Lors de sa construction, le JSON global est pass� en constructeur. Puis, chaque
membre de l'arbre requiert la r�cup�ration de sa version du JSON en fournissant son identifiant. Une fois l'identifiant re�u et le fragment envoy�, le syst�me se place dans le
fragment pass� pour envoyer les donn�es au membre suivant.
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
