#ifndef SIMPLEAIGUILLAGE_H
#define SIMPLEAIGUILLAGE_H

#include "BaseAiguillage.h"
#include <memory>

class AiguillageHandler;
class Alimentation;
class SimpleAiguillage : public BaseAiguillage
{
    public:
        enum PinState
        {
            Activated,
            Unactivated,
            Undefined
        };
        PinState getInvertedPinState(PinState toInvert);
        struct SinglePinModePin
        {
            SinglePinModePin(int newPinNumber, PinState newPinState) : pinNumber(newPinNumber), pinState(newPinState){};
            SinglePinModePin(SinglePinModePin const& toCopy)
                {
                    pinNumber = toCopy.pinNumber;
                    pinState = toCopy.pinState;
                };
            SinglePinModePin(){pinNumber = 0; pinState = Undefined;};
            int pinNumber;
            PinState pinState;
        };
        /** Default constructor */
        SimpleAiguillage(std::vector<Direction> validDirections);
        SimpleAiguillage(std::vector<Direction> validDirections, nlohmann::json save);
        //SimpleAiguillage(nlohmann::json backup);
        /** Default destructor */
//        virtual ~SimpleAiguillage();
        virtual bool changeSens(Direction direction, std::vector<ErrorsAiguillage>& errors)=0;
        virtual bool changeSens(int direction, std::vector <ErrorsAiguillage> &errors)=0;
        virtual SinglePinModePin getPin(Direction direction)=0;//permet de récuperer le pin et le mode du pin (allumé ou éteint)
        virtual std::vector<int> getUsedPins();
        virtual void confirm();
        virtual void directionChanger();
        virtual std::string getStringifiedType();


    protected:
        std::map<Direction,SinglePinModePin> m_directionsMap;

        static int i_id;
        int getNextDisponibleId();
        int m_alimentationId, m_alimentationHandlerId;
    private:
        void registerJSONNode();
};

#endif // SIMPLEAIGUILLAGE_H
