#ifndef VW_OUTRO_HPP
#define VW_OUTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cVWOutro : public cDinamicLayer
{
private:

public:
	cVWOutro(cocos2d::Size size, cBaseMode *thisMode);
	~cVWOutro();

	void step(float dt);
	void reset();

};


#endif // !VW_OUTRO_HPP