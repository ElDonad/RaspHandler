#ifndef SINLGEPINMODECOMPATIBLEAIGUILLAGEHANDLER_H
#define SINLGEPINMODECOMPATIBLEAIGUILLAGEHANDLER_H

#include "AiguillageHandler.h"

/** @brief AiguillageHandler compatible SingleModePin.

Cette classe est la base de tous les AiguillageHandler compatibles avec la technologie de déclenchement SingleModePin.


*/
class SinglePinModeCompatibleAiguillageHandler : public AiguillageHandler
{
    public:

        struct Alimentation
        {
            int pinId;
            int aiguillageHandlerId;//!< L'aiguillageHandler qui s'en charge (et donc qui la contient)
            int alimentationId;
            std::string alimentationName;
            int state;
        };

        virtual std::vector<std::pair<nlohmann::json,std::weak_ptr<Alimentation> > > getAlimentations()=0;//!< @return la liste des alimentations disponibles.
        virtual void addAlimentation(int pinId, std::string alimentationName,nlohmann::json comp)=0;//!< AJoute une alimentation. Doit être implémenté par les handlers.
        virtual AiguillageHandlerSwitchingAlimState toggleAlim(int pinAlim, bool switchState = false)=0;
        virtual nlohmann::json getCompLocParamsAlimentation()=0;//!< idem que getCompLocParamsAiguillage() , mais pour les alimentations
        virtual void requestSimpleSwitch(SimpleAiguillage::SinglePinModePin toSwitch)=0;

    protected:

    private:
};

#endif // SINLGEPINMODECOMPATIBLEAIGUILLAGEHANDLER_H
