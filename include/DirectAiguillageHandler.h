#ifndef DIRECTAIGUILLAGEHANDLER_H
#define DIRECTAIGUILLAGEHANDLER_H

#include <AiguillageHandler.h>
#include <SinglePinModeCompatibleAiguillageHandler.h>
#include "AiguillageHandlerEvent.h"

#ifdef RASP
#include <wiringPi.h>
#endif

class AiguillageManager;
class DirectAiguillageHandler : public SinglePinModeCompatibleAiguillageHandler
{
    public:
        /** Default constructor

        @param parent : l'AiguillageManager qui le stocke. */
        DirectAiguillageHandler(AiguillageManager* parent, std::string name = "DirectAiguillageHandler");
        /** Default destructor */
        virtual ~DirectAiguillageHandler();
        virtual void launch();
        virtual AiguillageHandlerActivatingAiguillageState switchAiguillage(int aiguillage, BaseAiguillage::Direction direction);
        virtual AiguillageHandlerSwitchingAlimState toggleAlim(int pinAlim, bool switchState = false);
        virtual void claimPin(int pinToClaim);

        virtual std::vector<std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage> > > getAiguillages();
        virtual std::pair<nlohmann::json,std::weak_ptr<BaseAiguillage> > findAiguillageById(int id,bool* founded = nullptr);
        virtual void addAiguillage(std::shared_ptr<BaseAiguillage> aiguillage, nlohmann::json comp = nlohmann::json());

        virtual std::vector<std::pair<nlohmann::json,std::weak_ptr<Alimentation> > > getAlimentations();
        virtual void addAlimentation(int pinId, std::string alimentationName, nlohmann::json comp = nlohmann::json());

        virtual nlohmann::json getCompLocParamsAiguillage();
        virtual nlohmann::json getCompLocParamsAlimentation();
        virtual void transmitEvent(std::shared_ptr<AiguillageHandlerEvent> event);
        virtual void requestSimpleSwitch(SimpleAiguillage::SinglePinModePin toSwitch);


    protected:
        std::vector<int> claimedPins;
        bool isPinClaimed(int pin);
        virtual void simpleSwitch(int pin, SimpleAiguillage::PinState sens);
        virtual void doubleSwitch(int pin);
        std::vector<std::shared_ptr<BaseAiguillage> > m_aiguillages;//!< Le conteneur des aiguillages sous la responsabilite de l'AiguillageHandler.

        std::vector<std::shared_ptr<Alimentation> > m_alimentations;
        static int i_nextAvailableId;
        int getNewAlimentationId();
        virtual void t_aiguillageChecker();


    private:
        static const std::vector<BaseAiguillage::AiguillageType> compatiblesAiguillages;
        std::vector<int> getUsedPins();
};

#endif // DIRECTAIGUILLAGEHANDLER_H
