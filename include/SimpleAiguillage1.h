#ifndef SIMPLEAIGUILLAGE1_H
#define SIMPLEAIGUILLAGE1_H

#include <SimpleAiguillage.h>

class AiguillageManager;
class SimpleAiguillage1 : public SimpleAiguillage
{
    public:
        struct AiguillageParams : public BaseAiguillage::AiguillageParams
        {
            SinglePinModePin leftDirectionPin;
            SinglePinModePin rightDirectionPin;
            Direction direction;//soit direction par défaut en constructeur, soit direction courante en récupérateur.
            int alimId, alimHandlerId;
        };
        /** Default constructor */
        SimpleAiguillage1(std::string name, int pin, std::weak_ptr<AiguillageHandler> parent, PinState defaultLeftPinState = PinState::Unactivated);
        SimpleAiguillage1(SimpleAiguillage1::AiguillageParams* params, std::weak_ptr<AiguillageHandler> parent);
        /** Default destructor */
        //virtual ~SimpleAiguillage1();
        virtual bool changeSens(Direction direction, std::vector<ErrorsAiguillage>& errors);
        virtual bool changeSens(int direction, std::vector<ErrorsAiguillage>& errors);
        virtual SinglePinModePin getPin(Direction direction);//permet de récuperer le pin et le mode du pin (allumé ou éteint)
        virtual Direction getCurrentDirection();
        virtual Direction getTargetDirection();

        virtual std::vector<int> getUsedPins();
        virtual void directionChanger();

    protected:

};

#endif // SIMPLEAIGUILLAGE1_H
