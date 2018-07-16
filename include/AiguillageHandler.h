#ifndef AIGUILLAGEHANDLER_H
#define AIGUILLAGEHANDLER_H
#include "BaseAiguillage.h"
#include "SimpleAiguillage1.h"
#include "AiguillageHandlerEvent.h"
#include "json.hpp"

#include "SFML/System.hpp"
#include <memory>
#define SWITTCH_TIMER 0.15
//#define RASP


class AiguillageManager;
//! La classe qui permet de manipuler directement les pins du raspberry

/** Cette classe contient les aiguillage qu'elle peut utiliser. C'est elle qui se charge de vérifier que ceux-ci ont besoin d'être togglé, et elle contient les fonctions qui s'en occupent.
Il est à noter que même si elle fournit à l'appel des fonctions getAiguillages(), getAlimentations() et similaires un json d'informations complémentaires, elle ne doit pas en avoir besoin pour identifier un aiguillage ! Seul le BaseAiguillage::m_id doit entrer en compte pour identifier un aiguillage (idem pour les alimentations)


*/
class AiguillageHandler
{
    public:
        enum AiguillageHandlerType
        {
            DirectAiguillageHandlerType,
            NetworkAiguillageHandlerType
        };
        enum AiguillageHandlerActivatingAiguillageState//!< Toutes les issues possibles lors de l'activation d'un aiguillage.
        {
            AiguillageDone,
            AiguillageInvalidPinError,
            UnsupportedAiguillageError,
            AiguillageInvalidSensError
        };

        enum AiguillageHandlerSwitchingAlimState//!< Toutes les issues possibles lors de l'allumage d'une alimentation.
        {
            AlimDone,
            AlimInvalidPinError
        };
        /** Default constructor */
        //AiguillageHandler(AiguillageManager* parent);
        /** Default destructor */
        virtual ~AiguillageHandler();
        //! Démarre le handler
        /**Fonction commune à tous les aiguillageHandler. Démarre les services et les rends opérationnels.
        */
        virtual void launch()=0;
        virtual AiguillageHandlerActivatingAiguillageState switchAiguillage(int aiguillageId, BaseAiguillage::Direction direction)=0;/**< Switch l'aiguillage concerné.

        Cette fonction se charge de changer la direction de l'aiguillage.
        @param aiguillage : l'aiguillage à activer.
        @return AiguillageHandlerActivatingAiguillageState::Done si tout c'est bien passé*/

        //virtual AiguillageHandlerSwitchingAlimState toggleAlim(int pinAlim, bool switchState = false)=0;
        virtual void addAiguillage(std::shared_ptr<BaseAiguillage> aiguillage, nlohmann::json comp)=0;

        /**Permet d'enregistrer correctement un pin.

        Cette fonction permet de préparer un pin à son utilisation en le passant en mode out et en l'éteignant. Il est recommandé de l'appeler pour chaque pin enregistré.
        @param pinToClaim : Le pin à mettre en place.

        */
        virtual void claimPin(int pinToClaim)=0;
        virtual std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage> > > getAiguillages()=0;
        virtual std::pair<nlohmann::json, std::weak_ptr<BaseAiguillage> > findAiguillageById(int id, bool* = nullptr)=0;//!< @param json : les caractéristiques de location supplémentaires
        int getId();
        bool initializeId(int id);//!< Used one time only to post-initialize the id. Can also be initialized in the constructor.
        std::string getName();
        bool isSinglePinModeCompatible();

        virtual nlohmann::json getCompLocParamsAiguillage()=0;//!< Cette fonction permet de retourner un json à remplir qui permettra de mieux caractériser la localisation des aiguillages.
        virtual void transmitEvent(std::shared_ptr<AiguillageHandlerEvent>event)=0;




    protected:
        AiguillageManager *m_parent;
        const static sf::Time switchDuration;
        //std::vector <std::shared_ptr<BaseAiguillage> > m_aiguillages;//la table des aiguillages
        //bool isValidTypeAiguillage(BaseAiguillage::AiguillageType aiguillageType[], int tableSize, BaseAiguillage::AiguillageType toAnalyze);//!< Petite fonction utilitaire qui signale si un aiguillage est compatible avec le handler.
        int m_id;//!<L'id attribué par l'AiguillageManager.
        bool m_isInitialized;//!< L'id est-il celui qui est attribué ?
        std::string m_nom;//!<Utilisé pour connaître l'utilité du Handler;
        bool m_isSinglePinModeCompatible;//!< Permet de déterminer si on peut utiliser toutes les fonctions relatives à l'utilisation des alimentations.
        std::shared_ptr<sf::Thread> m_aiguillageChecker;
        virtual void t_aiguillageChecker()=0;


    private:
};

#endif // AIGUILLAGEHANDLER_H
