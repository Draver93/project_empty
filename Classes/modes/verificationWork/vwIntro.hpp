#ifndef VW_INTRO_HPP
#define VW_INTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"


class cVWIntro : public cDinamicLayer
{
private:
	cBaseNode * listenNode;
	cButton *introBtn;
	cocos2d::Layer *layer;
public:
	cVWIntro(cocos2d::Size size, cBaseMode *thisMode);
	~cVWIntro();

	void step(float dt);
	void reset();
};


#endif // !VW_INTRO_HPP