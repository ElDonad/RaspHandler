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

/** Cette classe contient les aiguillage qu'elle peut utiliser. C'est elle qui se charge de v�rifier que ceux-ci ont besoin d'�tre toggl�, et elle contient les fonctions qui s'en occupent.
Il est � noter que m�me si elle fournit � l'appel des fonctions getAiguillages(), getAlimentations() et similaires un json d'informations compl�mentaires, elle ne doit pas en avoir besoin pour identifier un aiguillage ! Seul le BaseAiguillage::m_id doit entrer en compte pour identifier un aiguillage (idem pour les alimentations)


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
        //! D�marre le handler
        /**Fonction commune � tous les aiguillageHandler. D�marre les services et les rends op�rationnels.
        */
        virtual void launch()=0;
        virtual AiguillageHandlerActivatingAiguillageState switchAiguillage(int aiguillageId, BaseAiguillage::Direction direction)=0;/**< Switch l'aiguillage concern�.

        Cette fonction se charge de changer la direction de l'aiguillage.
        @param aiguillage : l'aiguillage � activer.
        @return AiguillageHandlerActivatingAiguillageState::Done si tout c'est bien pass�*/

        //virtual AiguillageHandlerSwitchingAlimState toggleAlim(int pinAlim, bool switchState = false)=0;
        virtual void addAiguillage(std::shared_ptr<BaseAiguillage> aiguillage, nlohmann::json comp)=0;

        /**Permet d'enregistrer correctement un pin.

        Cette fonction permet de pr�parer un pin � son utilisation en le passant en mode out et en l'�teignant. Il est recommand� de l'appeler pour chaque pin enregistr�.
        @param pinToClaim : Le pin � mettre en place.

        */
        virtual void claimPin(int pinToClaim)=0;
        virtual std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage> > > getAiguillages()=0;
        virtual std::pair<nlohmann::json, std::weak_ptr<BaseAiguillage> > findAiguillageById(int id, bool* = nullptr)=0;//!< @param json : les caract�ristiques de location suppl�mentaires
        int getId();
        bool initializeId(int id);//!< Used one time only to post-initialize the id. Can also be initialized in the constructor.
        std::string getName();
        bool isSinglePinModeCompatible();

        virtual nlohmann::json getCompLocParamsAiguillage()=0;//!< Cette fonction permet de retourner un json � remplir qui permettra de mieux caract�riser la localisation des aiguillages.
        virtual void transmitEvent(std::shared_ptr<AiguillageHandlerEvent>event)=0;




    protected:
        AiguillageManager *m_parent;
        const static sf::Time switchDuration;
        //std::vector <std::shared_ptr<BaseAiguillage> > m_aiguillages;//la table des aiguillages
        //bool isValidTypeAiguillage(BaseAiguillage::AiguillageType aiguillageType[], int tableSize, BaseAiguillage::AiguillageType toAnalyze);//!< Petite fonction utilitaire qui signale si un aiguillage est compatible avec le handler.
        int m_id;//!<L'id attribu� par l'AiguillageManager.
        bool m_isInitialized;//!< L'id est-il celui qui est attribu� ?
        std::string m_nom;//!<Utilis� pour conna�tre l'utilit� du Handler;
        bool m_isSinglePinModeCompatible;//!< Permet de d�terminer si on peut utiliser toutes les fonctions relatives � l'utilisation des alimentations.
        std::shared_ptr<sf::Thread> m_aiguillageChecker;
        virtual void t_aiguillageChecker()=0;


    private:
};

#endif // AIGUILLAGEHANDLER_H
