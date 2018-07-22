#ifndef IW_INTRO_HPP
#define IW_INTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"


class cIWIntro : public cDinamicLayer
{
private:
	cBaseNode * listenNode;
	cButton *introBtn;
	cocos2d::Layer *layer;
public:
	cIWIntro(cocos2d::Size size, cBaseMode *thisMode);
	~cIWIntro();

	void step(float dt);
	void reset();
};


#endif // !TW_INTRO_HPP