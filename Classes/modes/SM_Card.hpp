#ifndef SM_CARD_HPP
#define SM_CARD_HPP

#include "baseClasses/BaseMode.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/UserDefault.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "baseClasses/cardRoulette/CardRoulette.hpp"
#include "cards/Cards.hpp"

class cSMCardIntro : public cDinamicLayer
{
private:
	cButton *examGame;
	cocos2d::LayerColor *layer;
	cBaseNode *listenNode;

	//Cards
	std::vector<cocos2d::Color3B> gradeColors;
	int prevColor, nextColor;
	cWindow *dinamicCard;
	float timeChange;

	int turnCount;
public:
	cSMCardIntro(cocos2d::Size size, cBaseMode *thisMode);
	~cSMCardIntro();
	void step(float dt);
	void reset();
};
class cSMCardGame : public cDinamicLayer
{
private:
	int dayCost;
	float addtime;
	cGameField *field;

	int turnCount, turns;
	int elementCount;
	std::vector<int> cardDropInfo;

	std::vector<int> cardRares;
	std::vector<cocos2d::Node*> sCardNodes;

	cWindow *infoField, *btmBar, *cardField, *progressBarFill;
	cocos2d::Size fieldSize, ifSize, cfSize, lbSize;
	cocos2d::Label *lCurrentScore;
private:
	void updateInfoBar();
	void updateGameStats(float dt);
	void checkGameFinish();
	void updateCardPos();
	void addCard(int rare);
public:
	cSMCardGame(cocos2d::Size size,int turnCount,int dayCost, std::vector<int> cardDropInfo);
	~cSMCardGame();
	void step(float dt);
	void reset();
};
class cSMCardOutro : public cDinamicLayer
{
private:
	cocos2d::Node *sCard, *infoWnd;
	float timeFade;
	cWindow *resultWnd, *scoreWnd, *markWnd;
	cocos2d::Size szScoreWnd, szMarkWnd;

	cCardRoulette *roullet;
	cStudentCard *studentCard;
public:
	cSMCardOutro(int turns, int turnCounts, int dayCost, std::vector<int> cardsRares);
	~cSMCardOutro();

	void step(float dt);
	void reset();
};

class cSMCard : public cBaseMode
{
public:
	int turnCount, turns;
	int dayCost;
	int cardRareOpen;
	std::vector<int> cardDropInfo;
	std::vector<int> cardRares;

public:
	cSMCard(std::string modeName);
	~cSMCard();
	cDinamicLayer* getModeIntro(cocos2d::Size size);
	cDinamicLayer* getGameNode(cocos2d::Size size);
	cDinamicLayer* getModeOutro(cocos2d::Size size);
	std::string generateMode(std::string modeName);
};

#endif //SM_CARD_HPP