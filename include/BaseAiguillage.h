#ifndef BASEAIGUILLAGE_H
#define BASEAIGUILLAGE_H

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "json.hpp"
#include "TreeSerializer.h"

class AiguillageHandler;
class BaseAiguillage
{
    public:
        enum Direction
        {
            Invalid,
            Left,
            Right,
            Middle
        };

        enum AiguillageType
        //De la forme <type de connection> <nom> <nombre de pins utilisés>
        {

                SimpleAiguillage1,
                SimpleTripleAiguillage2,
                DoubleAiguillage2,
                DoubleTripleAiguillage4
        };

        enum AiguillageMode
        {
            Double,
            Simple
        };

        enum ErrorsAiguillage
        {
            InvalidDirection,
            AlreadyInTargetDirection,
            Indisponible
        };

        enum IndisponibilityReasons
        {
            AlimentationNotDisponible,
            UserDecision,//!<Si un utilisateur souhaite empecher l'utilisation de l'aiguillage par mesure de sécurité, par exemple
            AiguillageHandlerIssue//!< En cas de problème au sein de l'aiguillageHandler par exemple (une déconnexion ?)

        };

        /** Une fonction qui retourne un json simplifié de l'aiguillage. Utile pour les communications réseau.

        Structure du fichier :

        - type : le type de l'aiguillage, stringified
        - id : l'id de l'aiguillage
        - current_direction : la direction courante de l'aiguillage.
        - tag : le nom de l'aiguillage.

        d'autres données peuvent être ajoutées plus tard, en utilisant toujours le système du serializer.

        */
        virtual nlohmann::json getClassicSerializedData();

        void setIndisponible(IndisponibilityReasons reason);
        void setIndisponible(std::vector<IndisponibilityReasons> reasons);
        bool setDisponible(IndisponibilityReasons reason);//!< Supprime la raison indiquée du tableau des raisons d'indisponibilité. Il est à noter que lorsuq'il n' a plus d'indisponiblilité, l'aiguillage est à nouveau considéré comme disponible, et donc va se réactiver en confirmant ses positions. @return l'état (disponible ou indisponible) à la fin de l'opération.

        bool isDisponible();
        /** Default constructor */
        BaseAiguillage(std::vector<Direction>validDirections);
        BaseAiguillage(std::vector<Direction> validDirections, nlohmann::json saveData);
        /**Constructeur à partir de noeud de sauvegarde */
        //BaseAiguillage(nlohmann::json backup);

        virtual bool changeSens(Direction newDirection, std::vector<ErrorsAiguillage> &errors)=0;
        virtual bool changeSens(int direction, std::vector<ErrorsAiguillage> &errors)=0;

        virtual std::vector<int> getUsedPins()=0;

        virtual Direction getCurrentDirection()=0;
        virtual Direction getTargetDirection()=0;//!< Utile pour connaître la direction qu'est en train de prendre un aiguillage si le processus de changement prend du temps...
        virtual void confirm()=0;

        virtual AiguillageType getType();
        std::weak_ptr<AiguillageHandler> getParentAiguillageHandler();

        virtual std::string getTag();
        int getId();//!< @return l'id de l'aiguillage utilise par l'AiguillageHandler.
        AiguillageMode getMode();


        /** Default destructor */
        virtual ~BaseAiguillage();

        struct AiguillageParams //!< cette struct est utilisée dans les évènements pour transmettre les paramètres. Plus pratique qu'une map...
        {
            std::string name;
            virtual ~AiguillageParams()=default;
        };
        virtual std::vector<Direction> getValidDirections();
        virtual void directionChanger()=0;
        nlohmann::json save();
        virtual std::string getStringifiedType();

        static Direction convertIntIntoDirection(int direction);
        static int convertDirectionIntoInt(Direction direction);

        static std::string convertDirectionIntoString(Direction direction);
        static Direction convertStringIntoDirection(std::string direction);

        nlohmann::json getCompInformations();
        void setCompInformation(nlohmann::json newInfo, std::string identifier);


    protected:
        AiguillageMode m_mode;
        AiguillageType m_type;

        Direction m_currentDirection;
        Direction m_targetDirection;
        const std::vector<Direction> m_validDirections;

        bool m_isStopping;//!< Non stockés en mémoire car doit être défini en cours d'éxecution.
        bool m_isDisponible;//!< Non stockés en mémoire car doit être défini en cours d'éxecution.
        std::vector<IndisponibilityReasons> m_indisponibilityReasons;//!< Non stockés en mémoire car doit être défini en cours d'éxecution.
        std::string m_tag;
        int m_id;//!<l'id attribué par l'AiguillageHandler.
        std::weak_ptr<AiguillageHandler> m_parent;

        void mergeJSon(std::shared_ptr<nlohmann::json> main, nlohmann::json& toAdd);
        TreeSerializer<BaseAiguillage> m_serializer;//!< Utilisé uniquement à des fins de ctockage.
        TreeSerializer<BaseAiguillage> m_classicSerializer; //!< Utilisé pour véhiculer toutes les informations publiques de l'aiguillage.
        nlohmann::json m_complementaryInformations;

    private:
        void registerJSONNode();
};

#endif // BASEAIGUILLAGE_H
