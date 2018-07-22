#ifndef TW_OUTRO_HPP
#define TW_OUTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cTWOutro : public cDinamicLayer
{
private:

public:
	cTWOutro(cocos2d::Size size, cBaseMode *thisMode);
	~cTWOutro();

	void step(float dt);
	void reset();

};


#endif // !TW_OUTRO_HPP