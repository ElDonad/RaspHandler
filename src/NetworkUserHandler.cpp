#include "NetworkUserHandler.h"
#include "EventHandler.h"

using namespace std;

NetworkUserHandler::NetworkUserHandler(EventHandler* parent, nlohmann::json saveNode) : m_listener([](const std::string& strLogMsg) {/*std::cout<<strLogMsg<<std::endl;*/}, "2121")
{
    m_parentEventHandler = parent;
    m_stop = false;
    if (saveNode != nlohmann::json())//sauvegarde requise
    {
        //Sauvegarde data, plus tard...
    }

}

void NetworkUserHandler::launch()
{
    m_serverThread = new std::thread(&NetworkUserHandler::m_UserHandlerThread, this);
}

void NetworkUserHandler::m_UserHandlerThread()
{
    while (m_stop == false)
    {
        serverTick();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

nlohmann::json NetworkUserHandler::processItem(nlohmann::json toProcess, nlohmann::json client)
{
    if (toProcess["event_type"] == "GetAiguillages")
    {
        std::cout<<"Demande de récupération d'aiguillage recue"<<std::endl;
        nlohmann::json toReturn;
        toReturn["event_type"] = "GotAiguillages";
        toReturn["aiguillages"] = nlohmann::json::array();
        std::vector<std::pair<nlohmann::json, std::weak_ptr<BaseAiguillage>>> aiguillages = m_parentEventHandler->getAiguillage();
        for (auto it = aiguillages.begin(); it != aiguillages.end(); ++it)
        {
            toReturn["aiguillages"][std::distance(aiguillages.begin(), it)] = it->second.lock()->getClassicSerializedData();
            toReturn["aiguillages"][std::distance(aiguillages.begin(), it)]["aiguillage_handler_id"] = it->second.lock()->getParentAiguillageHandler().lock()->getId();

        }
        std::cout<<toReturn.dump(4);
        return toReturn;

    }

    else if (toProcess["event_type"] == "SwitchAiguillage")
    {
        std::cout<<"switching aiguillage..."<<std::endl;
        std::shared_ptr<UserEvent> event (new UserEvent(UserEvent::UserEventTypes::ChangeAiguillageState, client));
        event->changeAiguillageStateEvent.aiguillageHandlerId = toProcess["event_handler_id"];
        event->changeAiguillageStateEvent.aiguillageId = toProcess["aiguillage_id"];
        event->changeAiguillageStateEvent.targetState = BaseAiguillage::convertStringIntoDirection(toProcess["target_direction"]);
        m_parentEventHandler->transmitEvent(event);
        return nlohmann::json();
    }

    else if (!toProcess.empty())
    {
        std::cout<<"erreur de réception de paquet"<<std::endl;
    }

    return nlohmann::json();
}

void NetworkUserHandler::serverTick()
{
    ASocket::Socket* newClient = new ASocket::Socket;

    if (m_listener.Listen(*newClient, 5))
    {
        std::cout<<"> Nouveau client recu."<<std::endl;
        std::string headerMessage = socketReceive(newClient, true)[0];
        nlohmann::json gotHeader = nlohmann::json::parse(headerMessage);
        nlohmann::json gotHeaderEvent;
        gotHeaderEvent["event_type"] = "Logged";
        if (gotHeader["event_type"] == "SendHeadMessage")
        {
            if (gotHeader["wish_recover"] == true && !isIdTaken(gotHeader["previous_key"]))
            {
                for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
                {
                    if (it->first.id == gotHeader["previous_key"])
                    {
                        std::string key = gotHeader["previous_key"];
                        m_clients.push_back(std::pair<Client, ASocket::Socket*>(key, newClient));
                        gotHeaderEvent["is_recovered"] = true;
                        gotHeaderEvent["key"] = key;
                    }
                }
                std::cout<<"> Compte du client recupere"<<std::endl;
            }
            else
            {
                Client truc(getNewId());
                m_clients.push_back(std::pair<Client, ASocket::Socket*>(truc, newClient));
                std::cout<<"Compte du client cree"<<std::endl;
                gotHeaderEvent["is_recovered"] = false;
                gotHeaderEvent["key"] = truc.id;
            }
            m_listener.Send(*newClient, gotHeaderEvent.dump());
        }
        else
        {
            std::cout<<"erreur de reception"<<std::endl;
        }
    }

    //2. Gestion des évènements
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        std::vector<std::string> received = socketReceive(it->second);
        for (auto it2 = received.begin(); it2 != received.end(); ++it2)
        {
            nlohmann::json toProcess = nlohmann::json::parse(*it2);
            nlohmann::json sender;
            sender["id"] = it->first.id;
            nlohmann::json toSend = processItem(toProcess, sender);
            if (!toSend.empty())
            {
                m_listener.Send(*(it->second), toSend.dump());
                std::cout<<"Donnees envoyees"<<std::endl;
            }
        }
    }


}

NetworkUserHandler::~NetworkUserHandler()
{
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        delete it->second;
    }
}

void NetworkUserHandler::receiveAnswer(std::shared_ptr<BaseEvent> event)
{
    ASocket::Socket* client = nullptr;
    if (event->getReceiver()["id"] != 0)
    {
        for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
        {
            if(it->first.id == event->getReceiver()["id"])
            {
                client = it->second;
            }
        }
    }
    switch (event->getType())
    {

    case BaseEvent::AiguillageAdded:
        {
            nlohmann::json toSend;
// TODO (Elie#9#): Event provisoire pour forcer le rechargement. A terme faire quelque chose de plus propre.

            toSend["event_type"] = "dirty";
            m_listener.Send(*client, toSend.dump());
            break;
        }

    case BaseEvent::AiguillageStateChanged:
        {
            nlohmann::json toSend;

            toSend["event_type"] = "dirty";
            broadcast(toSend.dump());
            break;
        }
    }
}

nlohmann::json NetworkUserHandler::save()
{
    return nlohmann::json({});
}

std::string NetworkUserHandler::getNewId()
{
    std::string toReturn;
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int loop = 0; loop != 10; ++loop)
    {
        toReturn += characters[rand() % (sizeof(characters) - 1)];
    }
    return toReturn;
}

bool NetworkUserHandler::isIdTaken(std::string id)
{
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        if (it->first.id == id) return true;
    }
    return false;
}

std::vector<std::string> NetworkUserHandler::socketReceive(ASocket::Socket* toReceive, bool wait)
{
    char buffer[1024] = {0};
    std::fill( buffer, buffer + sizeof( buffer ), 0 );
    std::vector<std::string> toReturn;
    std::string message;
    int received;//taille totale recue
    int hasToTransfer;// retour direct de la socket, utile dans ce scope car utilisé dans la boucle conditionnelle

    do
    {
        std::size_t expectedArg;
        hasToTransfer = ASocket::SelectSockets(toReceive, 1, 5, expectedArg);
        if (hasToTransfer == 1){
            received = m_listener.Receive(*toReceive, buffer, 1023, false);
            message.append(buffer, 0, received);
        }
        else if (hasToTransfer == 0) continue;
        else if (hasToTransfer == -1) std::cerr<<"Erreur lors de lecture de la socket"<<std::endl;
        std::fill( buffer, buffer + sizeof( buffer ), 0 );
    }while (hasToTransfer != 0 || wait && message.size() == 0);
    if (!message.empty())
    {

        for (auto it = message.begin(); it != message.end() - 1; ++it)
        {
            if (*it == '\0003')
            {
                toReturn.push_back(std::string(message.begin(), it));
                message = std::string(it + 1, message.end());
            }
        }
        toReturn.push_back(std::string(message.begin(), message.end() - 1));
    }


    return toReturn;
}

void NetworkUserHandler::broadcast(std::string toSend)
{
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        m_listener.Send(*(it->second), toSend);
    }
}


