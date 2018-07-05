#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <string>
#include <vector>
#include "cocos2d.h"

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/BaseNode.hpp"
#include "cards/Cards.hpp"
#include "baseClasses/digitEx/cDigit.hpp"

struct cardInfo
{
	int state;
	int pos;
	cCard* pCard;
	cardInfo()
	{
		pos = -1;
		state = -1;
		pCard = nullptr;
	}
};
struct cardVisual
{
	cCard* pCard;
	cocos2d::Node *node;
	cocos2d::Node *nodeBack;
	cocos2d::Size originSize;
	float originScale;
	cocos2d::Vec2 originPos;
	cDigit x, y;
	cardVisual()
	{
		originScale = -1;
		pCard = nullptr;
		node = nullptr;
		nodeBack = nullptr;
	}
	~cardVisual()
	{
	};
	void update(float dt, float indent)
	{
		x.update(dt);
		y.update(dt);
		if (!x.isProgress())
		{
			int proc = rand() % 100;
			float ind = (indent / 100.0f) * proc - indent / 2.0f;

			x.set(ind, (rand() % 4) + 1);
		}
		if (!y.isProgress())
		{
			int proc = rand() % 100;
			float ind = (indent / 100.0f) * proc - indent / 2.0f;

			y.set(ind, (rand() % 4) + 1);
		}
		node->setPosition(originPos.x + x.get(), originPos.y + y.get());
	};
};

class cCardInventory : public cBaseNode
{
private:
	std::vector<cardInfo> stateCards; // state of a cards
	std::vector<cardInfo*> pSelectedCards; // selected cards
	std::vector<cardInfo*> pActiveCards; // active cards
	std::vector<cocos2d::Sprite*> slots;
	std::vector<cButton*> removeBtns;

	cBaseNode *cardList, *slotList;
	float widthCard, hightCard;

// new
	cocos2d::Sprite *btnAdd, *checkmark;


	cocos2d::Vec2 pickPoint;
	cocos2d::Vec2 pickPos;
	cDigit listPos[2];
	float lastListPos[2];
	int currentPos, centerCard, centerSC;
	cocos2d::Size size;

	std::vector<cardVisual*> elements;
	float wait;
	float time_points[5];
	bool isPress;

	void load();

	void updateList();
	void updateSlots();
	void updateStates();
	void selectCard();
public:
	void hide();
	void show();

	void save();
	cCardInventory(cocos2d::Size invSize);
	~cCardInventory();

};

#endif // !INVENTORY_HPP
