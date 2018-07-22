#ifndef T_OUTRO_HPP
#define T_OUTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cTOutro : public cDinamicLayer
{
private:

public:
	cTOutro(cocos2d::Size size, cBaseMode *thisMode);
	~cTOutro();

	void step(float dt);
	void reset();

};


#endif // !T_OUTRO_HPP