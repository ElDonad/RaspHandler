#include <iostream>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Base.h"

using namespace std;

int main()
{
    string input;
    bool network = false;
    bool loadFromSave = false;
    bool debugMode = false;

    Base base;

    cout<<"+++ Système de gestion d'aiguillage +++"<<endl;
    cout<<"Bienvenue ! Tapez \"help\" pour obtenir de l'aide"<<endl;
    cin>>input;

    if (input == "run")
    {
        cout<<">Programme demarre"<<endl<<endl;
        base.launch();
    }
    cout<<"Fonction main retournée"<<endl;
    return 0;
}
