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
        //De la forme <type de connection> <nom> <nombre de pins utilis�s>
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
            UserDecision,//!<Si un utilisateur souhaite empecher l'utilisation de l'aiguillage par mesure de s�curit�, par exemple
            AiguillageHandlerIssue//!< En cas de probl�me au sein de l'aiguillageHandler par exemple (une d�connexion ?)

        };

        /** Une fonction qui retourne un json simplifi� de l'aiguillage. Utile pour les communications r�seau.

        Structure du fichier :

        - type : le type de l'aiguillage, stringified
        - id : l'id de l'aiguillage
        - current_direction : la direction courante de l'aiguillage.
        - tag : le nom de l'aiguillage.

        d'autres donn�es peuvent �tre ajout�es plus tard, en utilisant toujours le syst�me du serializer.

        */
        virtual nlohmann::json getClassicSerializedData();

        void setIndisponible(IndisponibilityReasons reason);
        void setIndisponible(std::vector<IndisponibilityReasons> reasons);
        bool setDisponible(IndisponibilityReasons reason);//!< Supprime la raison indiqu�e du tableau des raisons d'indisponibilit�. Il est � noter que lorsuq'il n' a plus d'indisponiblilit�, l'aiguillage est � nouveau consid�r� comme disponible, et donc va se r�activer en confirmant ses positions. @return l'�tat (disponible ou indisponible) � la fin de l'op�ration.

        bool isDisponible();
        /** Default constructor */
        BaseAiguillage(std::vector<Direction>validDirections);
        BaseAiguillage(std::vector<Direction> validDirections, nlohmann::json saveData);
        /**Constructeur � partir de noeud de sauvegarde */
        //BaseAiguillage(nlohmann::json backup);

        virtual bool changeSens(Direction newDirection, std::vector<ErrorsAiguillage> &errors)=0;
        virtual bool changeSens(int direction, std::vector<ErrorsAiguillage> &errors)=0;

        virtual std::vector<int> getUsedPins()=0;

        virtual Direction getCurrentDirection()=0;
        virtual Direction getTargetDirection()=0;//!< Utile pour conna�tre la direction qu'est en train de prendre un aiguillage si le processus de changement prend du temps...
        virtual void confirm()=0;

        virtual AiguillageType getType();
        std::weak_ptr<AiguillageHandler> getParentAiguillageHandler();

        virtual std::string getTag();
        int getId();//!< @return l'id de l'aiguillage utilise par l'AiguillageHandler.
        int setId(int id);//TODO : à supprimer et remplacer par un système d'id plus viable.
        AiguillageMode getMode();


        /** Default destructor */
        virtual ~BaseAiguillage();

        struct AiguillageParams //!< cette struct est utilis�e dans les �v�nements pour transmettre les param�tres. Plus pratique qu'une map...
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

        bool m_isStopping;//!< Non stock�s en m�moire car doit �tre d�fini en cours d'�xecution.
        bool m_isDisponible;//!< Non stock�s en m�moire car doit �tre d�fini en cours d'�xecution.
        std::vector<IndisponibilityReasons> m_indisponibilityReasons;//!< Non stock�s en m�moire car doit �tre d�fini en cours d'�xecution.
        std::string m_tag;
        int m_id;//!<l'id attribu� par l'AiguillageHandler.
        std::weak_ptr<AiguillageHandler> m_parent;

        void mergeJSon(std::shared_ptr<nlohmann::json> main, nlohmann::json& toAdd);
        TreeSerializer<BaseAiguillage> m_serializer;//!< Utilis� uniquement � des fins de ctockage.
        TreeSerializer<BaseAiguillage> m_classicSerializer; //!< Utilis� pour v�hiculer toutes les informations publiques de l'aiguillage.
        nlohmann::json m_complementaryInformations;

    private:
        void registerJSONNode();
};

#endif // BASEAIGUILLAGE_H
