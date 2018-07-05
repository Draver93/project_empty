#ifndef SM_LVLUP_HPP
#define SM_LVLUP_HPP

#include "baseClasses/BaseMode.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/UserDefault.hpp"

struct sSubjectState
{
	int type;
	float goal;
	float score;
	cWindow* progressBar;
};

class cSMLvlUpIntro : public cDinamicLayer
{
private:
	cButton *examGame;
	cocos2d::LayerColor *layer;
	cBaseNode *listenNode;
	std::vector<cButton*> vecSubject;
	std::vector<int> activeSubjects;
public:
	cSMLvlUpIntro(cocos2d::Size size, cBaseMode *thisMode);
	~cSMLvlUpIntro();
	void step(float dt);
	void reset();
};
class cSMLvlUpGame : public cDinamicLayer
{
private:
	int dayCost;
	cGameField *field;
	float addtime;

	std::vector<float> playerSkills;
	int turnCount, turns;
	int elementCount;

	void updateInfoBar();
	void updateGameStats(float dt);
	void checkGameFinish();

private:
	cWindow *turnBarFill;
	cocos2d::Size turnBarSize;

	std::vector<sSubjectState> subjects;
	cocos2d::Size bSize;
public:
	cSMLvlUpGame(cocos2d::Size size,int turnCount,int dayCost, std::vector<int> inSubjects, std::vector<float> inPlayerSkills);
	~cSMLvlUpGame();
	void step(float dt);
	void reset();
};
class cSMLvlUpOutro : public cDinamicLayer
{
private:
public:
	cSMLvlUpOutro();
	~cSMLvlUpOutro();

	void step(float dt);
	void reset();
};

class cSMLvlUp : public cBaseMode
{
public:
	int turnCount;
	std::vector<int> subjects;
private:
	int turns;
	float currentScore;
	int dayCost;

	std::vector<float> playerSkills;
	std::vector<int> gameLevel;
public:
	cSMLvlUp(std::string modeName);
	~cSMLvlUp();
	cDinamicLayer* getModeIntro(cocos2d::Size size);
	cDinamicLayer* getGameNode(cocos2d::Size size);
	cDinamicLayer* getModeOutro(cocos2d::Size size);
	std::string generateMode(std::string modeName);
};

#endif // !SM_LVLUP_HPP
