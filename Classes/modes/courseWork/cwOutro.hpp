#ifndef CW_OUTRO_HPP
#define CW_OUTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cCWOutro : public cDinamicLayer
{
private:

public:
	cCWOutro(cocos2d::Size size, cBaseMode *thisMode);
	~cCWOutro();

	void step(float dt);
	void reset();

};


#endif // !TW_OUTRO_HPP