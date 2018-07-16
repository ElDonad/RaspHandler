#ifndef NETWORKUSERHANDLER_H
#define NETWORKUSERHANDLER_H

#include <iostream>
#include <string>
#include <list>

#include <UserHandler.h>
#include <UserEvent.h>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>


class EventHandler;

class NetworkUserHandler : public UserHandler
{
    public:
        /** Default constructor */
        NetworkUserHandler(EventHandler* parent);
        /** Default destructor */
        virtual ~NetworkUserHandler();
        virtual void launch();
        virtual void receiveAnswer(std::shared_ptr<BaseEvent> answer);
        virtual void stop();

        struct Client
        {
            sf::TcpSocket* socket;
            sf::Clock previousConfirm;
            std::string name;
        };

        struct StoredEvent
        {
            Client *client;
            int id;
        };


    protected:
        sf::Thread* m_listenerThread;//Ce thread sert uniquement à recevoir les demandes de connexion.
        sf::TcpListener m_listenerListener;

        sf::Thread* m_networkHandlerThread;//Ce thread reçoit et traite les paquets que les clients enregistrés lui envoient.
        void m_listener();
        void m_networkHandler();

        sf::Thread* m_checkThread;//ce dernier thread gère les checks cycliques.
        void m_check();

        std::vector <Client> m_clients;
        std::vector <StoredEvent> m_storedEvents;

        sf::Mutex m_Mclients;//mutex pour m_clients ET m_clientsSelector
        sf::SocketSelector m_clientsSelector;

        std::vector <std::string> returnFormatedPacket(std::string packet);

        //p'tites fonctions utilitaires sympa
        void broadcast(std::string message);
        void sendToClient(const Client* client, std::string message);



    private:
};

#endif // NETWORKUSERHANDLER_H
