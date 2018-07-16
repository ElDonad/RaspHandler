#ifndef CONSOLEUSERHANDLER_H
#define CONSOLEUSERHANDLER_H

#include "UserHandler.h"
#include "UserEvent.h"
#include <iostream>
#include <string>
#include <exception>
#include "AiguillageHandler.h"

class EventHandler;
class ConsoleUserHandler : public UserHandler
{
    public:
        /** Default constructor */
        ConsoleUserHandler(EventHandler* parent);
        /** Default destructor */
        virtual ~ConsoleUserHandler();
        virtual void launch();
        virtual void receiveAnswer(std::shared_ptr<BaseEvent> answer);
        virtual void stop();

    protected:
        std::vector <int> m_storedEventId;
        sf::Thread *m_listenerThread;
        sf::Mutex m_Mentry;
        void m_listener();

        sf::Thread* m_handlerThread;
        void m_handler();

        std::string m_entry;
        bool m_entryChanged;

        bool m_isStopping;

        std::string safeWaitForEntry();
        struct m_Aiguillage
        {
            std::string tag;
            int pinG;
            int pinD;
            int direction;
        };

        std::vector<m_Aiguillage> m_storedAiguillages;
        sf::Mutex m_MstoredAiguillages;

        //UTILITAIRES
        bool isConcerned(int idToConfirm);

        bool m_isInDebugMode;

    private:
        std::string formatWithSpaces(int spaceMax, const std::string& nomAiguillage);
        void displayAiguillageHandlersList();
        void displayAlimentationsList();
        void displayAlimentationsList(int aiguillageHandlerId);
        void displayAiguillagesList();

        //utilitaire pour l'affichage
        struct DisplayAiguillage
        {
            std::string nom;
            std::string dir;
            std::string type;
        };
        int getSafeInt(std::string annonce = "Entrez un nombre : ");
};

#endif // CONSOLEUSERHANDLER_H
