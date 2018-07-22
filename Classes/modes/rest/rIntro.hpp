#ifndef R_INTRO_HPP
#define R_INTRO_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"


class cRIntro : public cDinamicLayer
{
private:
	cBaseNode * listenNode;
	cButton *introBtn;
	cocos2d::Layer *layer;
public:
	cRIntro(cocos2d::Size size, cBaseMode *thisMode);
	~cRIntro();

	void step(float dt);
	void reset();
};


#endif // !R_INTRO_HPP