#ifndef CONSOLEUSERHANDLER_H
#define CONSOLEUSERHANDLER_H

#include "UserHandler.h"
#include "UserEvent.h"
#include <iostream>
#include <string>
#include <exception>
#include "AiguillageHandler.h"

#include <thread>
#include <mutex>

class EventHandler;
class ConsoleUserHandler : public UserHandler
{
    public:
        /** Default constructor */
        ConsoleUserHandler(EventHandler* parent);
        ConsoleUserHandler(EventHandler* parent, nlohmann::json saveData);
        /** Default destructor */
        virtual ~ConsoleUserHandler();
        virtual void launch();
        virtual void receiveAnswer(std::shared_ptr<BaseEvent> answer);
        virtual void stop();
        virtual nlohmann::json save();

    protected:
        std::vector <int> m_storedEventId;
        std::thread *m_listenerThread;
        std::mutex m_Mentry;
        void m_listener();

        std::thread* m_handlerThread;
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
        std::mutex m_MstoredAiguillages;

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
