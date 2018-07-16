#include "NetworkUserHandler.h"
#include "EventHandler.h"

using namespace std;

NetworkUserHandler::NetworkUserHandler(EventHandler* parent)
{
//    m_listenerThread = new sf::Thread(NetworkUserHandler::m_listener, this);
//    m_networkHandlerThread = new sf::Thread(NetworkUserHandler::m_networkHandler, this);
//    m_checkThread = new sf::Thread(NetworkUserHandler::m_check, this);
//    m_stop = false;
//    m_parentEventHandler = parent;
}

NetworkUserHandler::~NetworkUserHandler()
{
//    delete m_listenerThread;
//    delete m_networkHandlerThread;
//    delete m_checkThread;
}

void NetworkUserHandler::m_listener()
{
//    if (m_listenerListener.listen(11111) != sf::Socket::Done)
//    {
//        cout<<"error socket"<<endl;
//    }
//
//    while (isStopping() != true)
//    {
//
//        sf::TcpSocket* client = new sf::TcpSocket();
//        try{
//
//            int truc = m_listenerListener.accept(*client);
//
//            if (truc == sf::TcpSocket::Error)
//                throw truc;
//        }
//        catch (sf::TcpSocket::Status const& e )
//        {
//            if (e == sf::TcpSocket::Error)
//                cout<<"Problème grave ! Moi pas savoir quoi faire"<<endl;
//            int t;
//            cin>>t;
//        }
//
//        Client clientStruct;
//        clientStruct.socket = client;
//
//        m_clients.push_back(clientStruct);
//        client->setBlocking(false);
//        m_clientsSelector.add(*client);
//
//        sf::Packet requestPacket;
//        requestPacket<<"1001|";
//        m_clients.back().socket->send(requestPacket);
//        sf::sleep(sf::milliseconds(5));
//
//    }
}

void NetworkUserHandler::m_check()
{
//    while (isStopping() != true)
//    {
//        //1. Vérifier qu'aucun des clients n'est en idle depuis plus de 5s :
//        m_Mclients.lock();
//        for (int loop = 0; loop != m_clients.size(); loop++)
//        {
//            if (m_clients[loop].previousConfirm.getElapsedTime() >= sf::Time(sf::seconds(100000)))
//            {
//                delete m_clients[loop].socket;
//                m_clients.erase(m_clients.begin() + loop);
//            }
//        }
//
//        //end
//        sf::sleep(sf::milliseconds(10));
//    }
}

void NetworkUserHandler::m_networkHandler()
{
//    while (isStopping() != true)
//    {
//        Client* selectedClient;//client en attente de réponse
//        //Gestion des events
//        m_Mclients.lock();
//        m_clientsSelector.wait(sf::milliseconds(50));
//        sf::TcpSocket* ready;
//        bool isSelected = false;
//        for (int loop = 0; loop != m_clients.size(); loop++)
//        {
//            ready = m_clients[loop].socket;
//            if (m_clientsSelector.isReady(*ready))
//                {
//                    selectedClient = &m_clients[loop];
//                    isSelected = true;
//                    break;
//                }
//        }
//        if (isSelected == false)
//        {
//            continue;
//        }
//
//        if (m_clients.size() == 0)
//        {
//            sf::sleep(sf::milliseconds(5));
//            continue;
//        }
//        sf::Packet packetReceived;
//        ready->receive(packetReceived);
////      cout<<"Packet recu : ";
//        string receivedData;
//        packetReceived>>receivedData;
//        cout<<receivedData<<endl;
//
//        vector <string> formatedData = returnFormatedPacket(receivedData);
//        int orderType = std::stoi (formatedData[0]);
//
//        switch (orderType)
//        {
//        case 2101://ajout d'aiguillage
//            {
//                vector<string> args;
//                args.push_back(formatedData[1]);
//                args.push_back(formatedData[2]);
//                args.push_back(formatedData[3]);
//
//                UserEvent event (UserEvent::AddAiguillage, args);
//                m_parentEventHandler->transmitEvent(event);
//                int eventId = event.getId();
//                m_storedEvents.push_back(StoredEvent{selectedClient, eventId});
//                break;
//            }
//
//        case 2109://suppression d'aiguillage
//            {
//                vector<string> args;
//                args.push_back(formatedData[1]);
//
//                UserEvent event(UserEvent::AddAiguillage, args);
//                m_parentEventHandler->transmitEvent(event);
//                int eventId = event.getId();
//                m_storedEvents.push_back(StoredEvent{selectedClient, eventId});
//                break;
//            }
//
//        case 2105://confirmer les positions
//            {
//                vector<string> args;
//
//                UserEvent event(UserEvent::ConfirmPositions, args);
//                m_parentEventHandler->transmitEvent(event);
//                int eventId = event.getId();
//                m_storedEvents.push_back(StoredEvent{selectedClient, eventId});
//                break;
//            }
//
//        case 9999://stopper
//            {
//                vector<string> args;
//                args.push_back(formatedData[1]);
//
//                UserEvent event(UserEvent::Stop, args);
//                m_parentEventHandler->transmitEvent(event);
//                int eventId = event.getId();
//                m_storedEvents.push_back(StoredEvent{selectedClient, eventId});
//                break;
//            }
//
//        case 1001://premier pattern
//            {
//                selectedClient->name = formatedData[1];
//                cout<<"Client " + selectedClient->name + " enregistré avec succès."<<endl;
//                break;
//            }
//
//        case 1010:
//            {
//
//            }
//
//        case 2510://change aiguillage state
//            {
//                vector<string>args;
//                args.push_back(formatedData[1]);
//                args.push_back(formatedData[2]);
//                UserEvent event(UserEvent::ChangeAiguillageState,args);
//                int event_id = event.getId();
//                m_parentEventHandler->transmitEvent(event);
//                m_storedEvents.push_back(StoredEvent{selectedClient, event_id});
//            }
//        }
//    }
}

void NetworkUserHandler::launch()
{
//    m_listenerThread->launch();
//    m_networkHandlerThread->launch();
//    m_checkThread->launch();
//    cout<<"> Network Handler launched"<<endl;
}

void NetworkUserHandler::receiveAnswer(std::shared_ptr<BaseEvent> base)
{
//    cout<<"Nouvel event envoyé : "<<base.getType()<<endl;
//    int eventId = base.getId();
//    Client *concernedClient;
//    bool isConcerned = false;
//    for (int loop = 0; loop != m_storedEvents.size(); loop++)
//    {
//        if (m_storedEvents[loop].id == eventId)
//        {
//            isConcerned == true;
//            concernedClient = m_storedEvents[loop].client;
//            break;
//        }
//    }
//
//        switch (base.getType())
//        {
//        case BaseEvent::AiguillageAdded://envoyé à tous les clients pour signaler qu'il leur faut update leur liste d'aiguillages
//            {
//                string message;
//                message = message + "2111|" + base.aiguillageAddedEvent.tag + "|" + std::to_string(base.aiguillageAddedEvent.pinG) + "|" + std::to_string(base.aiguillageAddedEvent.pinD) + "|" + std::to_string(base.aiguillageAddedEvent.direction);
//                broadcast(message);
//                break;
//            }
//
//        case BaseEvent::AddingAiguillageAborted:
//            {
//                string message = "2901|";
//                vector<string> errors;
//                for (int loop = 0; loop != errors.size(); loop++)
//                {
//                    message = message + errors[loop] + "|";
//                }
//                if (eventId == 0)
//                    broadcast(message);
//                else
//                    sendToClient(concernedClient, message);
//                break;
//            }
//
//        case BaseEvent::AddingAiguillageConfirmed://envoyé à la personne qui a ordonné l'ajout de l'aiguillage - pas de broadcast
//            {
//                string message;
//                message = message + "3111|" + base.addingAiguillageConfirmedEvent.tag;
//                sendToClient(concernedClient, message);
//                break;
//            }
//
//        case BaseEvent::AiguillageRemoved:
//            {
//                string message;
//                message = message + "2199|" + base.aiguillageRemovedEvent.tag;
//                broadcast(message);
//                break;
//            }
//
//        case BaseEvent::AiguillageStateChanged://broadcast pour le signaler à tous les clients
//            {
//                string message;
//                message = message + "2501|" + std::to_string(base.aiguillageStateChangedEvent.direction) + "|" + base.aiguillageStateChangedEvent.tag;
//                broadcast(message);
//                break;
//            }
//
//        case BaseEvent::Confirmed:
//            {
//                string message = "2115";
//                sendToClient(concernedClient, message);
//                break;
//            }
//
//        case BaseEvent::RemovingAiguillageAborted:
//            {
//                string message;
//                vector<string> errors = base.removingAiguillageAbortedEvent.errors;
//                message = message + "2991|";
//                for (int loop = 0; loop != errors.size(); loop++)
//                {
//                    message = message + errors[loop] + "|";
//                }
//                sendToClient(concernedClient, message);
//                break;
//            }
//
//        case BaseEvent::RemovingAiguillageConfirmed:
//            {
//                string message;
//                message + message + "3911|" + base.removingAiguillageConfirmedEvent.tag;
//                sendToClient(concernedClient, message);
//                break;
//            }
//
//        case BaseEvent::Stopping:
//            {
//                string message;
//                message = message + "3999|" + base.stoppingEvent.raison;
//                break;
//            }
//
//        }
}

vector <string> NetworkUserHandler::returnFormatedPacket(string packet)
{
//    vector <string> toReturn;
//    int nextSegment = 0;
//    int offset = 0;
//
//    for (int loop = 0; loop != packet.size(); loop++)
//    {
//        if (packet[loop] == '|')
//        {
//            toReturn.push_back(packet.substr(nextSegment + offset, loop - nextSegment - offset));
//            //cout<<"Formatage packet : nouveau fragment : "<<toReturn.back()<<endl;
//            nextSegment = loop;
//            offset=1;
//        }
//    }
//    toReturn.push_back(packet.substr(nextSegment + offset));
//    //cout<<"Formatage packet : fragment final : "<<toReturn.back()<<endl;
//    return toReturn;

}

void NetworkUserHandler::broadcast(string message)
{
//    sf::Packet broadcastPacket;
//    broadcastPacket<<message;
//    for (int loop = 0; loop != m_clients.size(); loop++)
//    {
//        m_clients[loop].socket->send(broadcastPacket);
//    }
}

void NetworkUserHandler::sendToClient(const Client* client, string message)
{
//    sf::Packet packetToSend;
//    packetToSend<<message;
//    client->socket->send(packetToSend);
}

void NetworkUserHandler::stop()
{
//    m_MisStopping.lock();
//    m_stop = true;
//    m_MisStopping.unlock();
//    m_listenerThread->wait();
//    m_checkThread->wait();
}


