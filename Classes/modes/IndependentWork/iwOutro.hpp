#ifndef IW_OUTRO_HPP
#define IW_OUTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cIWOutro : public cDinamicLayer
{
private:

public:
	cIWOutro(cocos2d::Size size, cBaseMode *thisMode);
	~cIWOutro();

	void step(float dt);
	void reset();

};


#endif // !TW_OUTRO_HPP