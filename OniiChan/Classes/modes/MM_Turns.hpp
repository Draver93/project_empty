#ifndef MM_TURNS_HPP
#define MM_TURNS_HPP
#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

class cMMTurnsGameNode : public cDinamicLayer
{
private:
	int dayCost;
	cGameField *field;

	float addtime;
	int turnCount, turns;
	float goal;
	float currentScore;
	int elementCount;
	std::vector<int> subjects;
	std::vector<float> playerSkills;

	std::vector<cStudentCard*> sCards;
	std::vector<cocos2d::Node*> sCardNodes;

	cTeacherCard* tCard;
	cocos2d::Node* tNode;

	cWindow* progressBarBg;
	cWindow* progressBarFill;
	cocos2d::Label *lMaxScore;
	cocos2d::Label* lMaxTurns;
	cocos2d::Label *lCurrentScore;
	cocos2d::Label *lCurrentTurns;
	cWindow *turnLabelBg;
	cWindow *turnBarBg;
	cWindow *turnBarFill;

	cWindow *cardField;
	cocos2d::Label *infoTable;
	std::vector<cocos2d::Label*> tableArray;

private:
	void addInTable(std::string text);
	void updateInfoBar();
	void updateGameStats(float dt);
	void checkGameFinish();
public:
	cMMTurnsGameNode(cocos2d::Size size, std::vector<float> inPlayerSkills, std::vector<int> inGameLevel, std::vector<int> inSubjects, cTeacherCard* teacherCard, float inGoal, int inTurnCount, int days);
	~cMMTurnsGameNode();

	void step(float dt);
	void reset();
};
class cMMTurnsIntroNode : public cDinamicLayer
{
private:
	cButton *examGame;
	cocos2d::LayerColor *layer;
	cBaseNode *listenNode;

public:
	cMMTurnsIntroNode(cocos2d::Size size, cBaseMode *thisMode);
	~cMMTurnsIntroNode();

	void step(float dt);
	void reset();
};
class cMMTurnsOutroNode : public cDinamicLayer
{
private:
	float cScore, cGoal;
	int cTurn, cTurnCount;

	cocos2d::Size aBarSize;
	cWindow *scoreBar;
	cocos2d::Label *sbLabel;
	float posBar;

	int dayCost;
	float barStepTime;
public:
	cMMTurnsOutroNode(cocos2d::Size size, int turns, int turnCount, float currentScore, float goal, int dayCost);
	~cMMTurnsOutroNode();

	void step(float dt);
	void reset();
	
};

class cMMTurns : public cBaseMode
{
public:	
	float goal;
	int turnCount;
	int turns;
	float currentScore;
	int dayCost;
	std::vector<float> playerSkills;
	std::vector<int> gameLevel;
	std::vector<int> subjects;
	cTeacherCard *teacherCard;

public:
	cMMTurns(std::string modeName);
	~cMMTurns();
	cDinamicLayer* getModeIntro(cocos2d::Size size);
	cDinamicLayer* getGameNode(cocos2d::Size size);
	cDinamicLayer* getModeOutro(cocos2d::Size size);
	std::string generateMode(std::string modeName);
};

#endif // !MM_TURNS_HPP
