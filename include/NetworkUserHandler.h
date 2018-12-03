#ifndef NETWORKUSERHANDLER_H
#define NETWORKUSERHANDLER_H

#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <json.hpp>

//pour la génération de clé aléatoire.
#include <random>
#include <algorithm>

#include <UserHandler.h>
#include <UserEvent.h>

#include <thread>
#include "TCPServer.h"


class EventHandler;

class NetworkUserHandler : public UserHandler
{

    public:

        struct Client
        {
            Client(std::string newId){
                id = newId;//!< cet id permet entre autres de savoir à quel utilisateur est adressé un event.
            }
            std::string id;
            nlohmann::json personalSave;
        };

        /** Default constructor */
        NetworkUserHandler(EventHandler* parent, nlohmann::json saveNode = nlohmann::json());
        /** Default destructor */
        virtual ~NetworkUserHandler();
        virtual void launch();
        virtual nlohmann::json save();
        virtual void receiveAnswer(std::shared_ptr<BaseEvent> event);

    protected:
        EventHandler* m_parentEventHandler;
        std::thread* m_serverThread;
        std::vector<std::pair<Client, ASocket::Socket*> > m_clients;

        CTCPServer m_listener;
        void m_UserHandlerThread();
        void serverTick();
        std::string getNewId();
        bool isIdTaken(std::string id);
        nlohmann::json processItem(nlohmann::json toProcess, nlohmann::json sender);//!< Retourne un json vide, ou à envoyer à l'utilisateur correspondant.

        std::vector<std::string> socketReceive(ASocket::Socket*, bool wait = false);

        void broadcast(std::string toSend);

};

#endif // NETWORKUSERHANDLER_H
