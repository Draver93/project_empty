#ifndef R_GAME_HPP
#define R_GAME_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cRGame : public cDinamicLayer
{
private:
	float addtime;
	int elementCount;
	int turnCount, turns;
	cGameField * field;
public:
	cRGame(cocos2d::Size size, cBaseMode *thisMode);
	~cRGame();

	void step(float dt);
	void reset();
};

#endif // !R_GAME_HPP