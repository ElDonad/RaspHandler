#include <iostream>
#include <string>
#include "Base.h"
#include "json.hpp"
//#define RASP

using namespace std;

int main()
{
    string input;
    bool network = false;
    bool loadFromSave = false;
    bool debugMode = false;
    #ifdef RASP
    wiringPiSetup();
    #endif // RASP

    std::shared_ptr<Base> base;

    cout<<"+++ Système de gestion d'aiguillage +++"<<endl;
    cout<<"Bienvenue ! Tapez \"help\" pour obtenir de l'aide"<<endl;
    cin>>input;

    if (input == "run")
    {
        cout<<">Programme demarre"<<endl<<endl;
        base = std::make_shared<Base>("save.json");
        base->launch();
    }
    else if (input == "rerun")
    {
        cout<<"> Programme demarre a partir de sauvegarde..."<<endl<<endl;
        nlohmann::json savedData;
        ifstream file ("save.json", ios::in);
        file>>savedData;
        base = std::make_shared<Base>("save.json", savedData);
        base->launch();
    }
    cout<<"Fonction main retournée"<<endl;
    return 0;
}
