#ifndef CARD_ROULETTE
#define CARD_ROULETTE

#include "cocos2d.h"
#include "string"
#include "vector"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseNode.hpp"

class cCardRoulette : public cocos2d::Node
{
public:
	cocos2d::Size cardSize;
private:
	bool enable;
	float time, speed;
	float inTime, moveTime, outTime;

	cocos2d::Size wndSize;
	std::vector<int> vecCards;
	std::vector<cBaseNode*> sCardNodes;
	cBaseNode *activeNode;
public:
	cCardRoulette(std::vector<int> cards, cocos2d::Size size);
	~cCardRoulette();
	void update(float dt) override;
	void start(float in, float move, float out, float spd);
	void reset();
	cBaseNode* getActiveCard() { return activeNode; }
	bool isActive() { if (time == 0 && speed == 0 && enable == false) return false; else return true; }
};

#endif // !CARD_ROULETTE
