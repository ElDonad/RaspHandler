#ifndef AIGUILLAGEMANAGER_H
#define AIGUILLAGEMANAGER_H

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "BaseAiguillage.h"
#include "UserEvent.h"
#include "SimpleAiguillage1.h"
#include "AiguillageHandler.h"
#include "SinglePinModeCompatibleAiguillageHandler.h"
#include "DirectAiguillageHandler.h"
#include "BaseEvent.h"

class Base;

//! \brief Le gestionnaire des AiguillageHandlers.
 /** Cette classe existe en un seul exemplaire dans un logiciel. Son r�le est de contenir les aiguillages, de g�rer les AiguillageHandler et de g�rer les exceptions �mises par ces derniers.
   @see AiguillageHandler()
 *
 */


class AiguillageManager
{
    /*
    Cette classe a pour fonction de contenir les AiguillageHandler et de leur donner des aiguillages � switcher.
    Les AiguillageHandler ne connaissent pas les aiguillages qu'ils peuvent switcher.

    Lors de la construction de l'aiguillage, l'utilisateur doit sp�cifier quel aiguillageHandler doit �tre utilis�.
    Pour cela il doit passer par un id, d�livr� par l'aiguillageManager, et qu'il peut r�cup�rer en connaissant le nom du handler.
    */

    public:


        //! \brief Constructeur de base
        /** @param base La classe Base � l'origine du programme

        */
        AiguillageManager(Base* base);
        AiguillageManager(Base* base, nlohmann::json saveData);
        /** Default destructor */
        virtual ~AiguillageManager();
        //! \brief Demarre le manager
        /** Cette fonction a pour r�le de lancer les divers threads et d'initialiser le gestionnaire.
            A appeler imp�rativement avant toute op�ration sur le gestionnaire.

        */
        void launch();//pour lancer le syst�me de threads
        void stop();
        //! execute l'�v�nement
        /** Process les �v�nements envoy�s par les utilisateurs

        @param event Un �v�nement utilisateur pass� par la base et la cha�ne d'appel des �v�nements
        */
        void proceedEvent(std::shared_ptr<UserEvent> event);//ordres d'ajouter aiguillage, switcher aiguillage,...
        void proceedEvent(std::shared_ptr<AiguillageHandlerEvent> event);//g�rer le retour de la part des aiguillageHandlers

        //! Donne un id de backup
        /** Cette fonction fournit un id de backup � tout aiguillageHandler qui le demande. Cet id devra �tre fourni dans l'event UnsuscribeEvent, puis repass� en backup pour qu'un aiguillageHandler puisse se reconnecter sans heurt.

        @return Un id unique pour chaque appel.

        */
        int getBackupId();
        std::vector<std::weak_ptr<AiguillageHandler> > getAiguillageHandlers();//!< @return la liste des aiguillageHandlers disponibles.
        std::weak_ptr<AiguillageHandler> findAiguillageHandlerById(int id, bool* success = nullptr);

        std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage>>> getAiguillages();//!<Renvoie la liste des aiguillages.
        std::pair<nlohmann::json,std::shared_ptr<BaseAiguillage>> findAiguillageById(int aiguillageHandler,int aiguillageId, bool* success);
        nlohmann::json save();
    protected:
        std::vector<std::shared_ptr<AiguillageHandler> > m_aiguillageHandlers;//!<Les aiguillageHandler.

        std::map<int, int> m_backupsIds;

        std::unique_ptr <Base> m_base;


        static int m_nextAvailableInt;
        int getNextId();//!< Fournit un id pour les aiguillageHandler

        static int m_nextBackupId;

        static int m_nextAlimentationId;
        int getAlimId();


    private:
};

#endif // AIGUILLAGEMANAGER_H
