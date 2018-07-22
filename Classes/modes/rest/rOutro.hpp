#ifndef R_OUTRO_HPP
#define R_OUTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cROutro : public cDinamicLayer
{
private:

public:
	cROutro(cocos2d::Size size, cBaseMode *thisMode);
	~cROutro();

	void step(float dt);
	void reset();

};


#endif // !R_OUTRO_HPP