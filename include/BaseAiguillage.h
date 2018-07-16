#ifndef BASEAIGUILLAGE_H
#define BASEAIGUILLAGE_H

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <SFML/System.hpp>

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
        void setIndisponible(IndisponibilityReasons reason);
        void setIndisponible(std::vector<IndisponibilityReasons> reasons);
        bool setDisponible(IndisponibilityReasons reason);//!< Supprime la raison indiqu�e du tableau des raisons d'indisponibilit�. Il est � noter que lorsuq'il n' a plus d'indisponiblilit�, l'aiguillage est � nouveau consid�r� comme disponible, et donc va se r�activer en confirmant ses positions. @return l'�tat (disponible ou indisponible) � la fin de l'op�ration.

        bool isDisponible();
        /** Default constructor */
        BaseAiguillage(std::vector<Direction>validDirections);
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


    protected:
        AiguillageMode m_mode;
        AiguillageType m_type;
        Direction convertIntIntoDirection(int direction);
        int convertDirectionIntoInt(Direction direction);

        std::string convertDirectionIntoString(Direction direction);
        Direction convertStringIntoDirection(std::string direction);

        Direction m_currentDirection;
        Direction m_targetDirection;
        const std::vector<Direction> m_validDirections;

        bool m_isStopping;
        bool m_isDisponible;
        std::vector<IndisponibilityReasons> m_indisponibilityReasons;
        std::string m_tag;
        int m_id;//!<l'id attribu� par l'AiguillageHandler.
        std::weak_ptr<AiguillageHandler> m_parent;


    private:
};

#endif // BASEAIGUILLAGE_H
