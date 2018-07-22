#ifndef SC_OUTRO_HPP
#define SC_OUTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cSCOutro : public cDinamicLayer
{
private:

public:
	cSCOutro(cocos2d::Size size, cBaseMode *thisMode);
	~cSCOutro();

	void step(float dt);
	void reset();

};


#endif // !SC_OUTRO_HPP