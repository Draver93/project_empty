#ifndef VW_GAME_HPP
#define VW_GAME_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cVWGame : public cDinamicLayer
{
private:
	float addtime, goal;
	int elementCount;
	int turnCount, turns;
	cDigit currentScore;
	cGameField* field;
	cTeacherCard* tCard;

	std::vector<floatElement*> elements;

	cocos2d::Size textScoreSize, testTurnSize;
	cocos2d::Label *scoreLabel, *turnLabel, *cardDesc;
	cocos2d::Size scoreSize, turnSize, cardSize, trCardSize;

	std::vector<int> subjects;
	std::vector<float> playerSkills;

	std::vector<cStudentCard*> sCards;

private:
	void updateInfoBar();
	void updateGameStats(float dt);
	void checkGameFinish();

public:
	cVWGame(cocos2d::Size size, cBaseMode *thisMode);
	~cVWGame();

	void step(float dt);
	void reset();
};

#endif // !VW_GAME_HPP