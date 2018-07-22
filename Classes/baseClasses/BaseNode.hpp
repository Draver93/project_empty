#ifndef BASE_NODE_HPP
#define BASE_NODE_HPP

#include "cocos2d.h"
#include "string"
#include "vector"

#include "baseClasses/digitEx/cDigit.hpp"

class cBaseNode : public cocos2d::Node
{
private:
public:
	cocos2d::EventListenerTouchOneByOne *listener;
	std::function<void(float)> secondUpdate;

	cBaseNode();
	~cBaseNode();

	void update(float dt) override;
	virtual void show() { this->setVisible(true); };
	virtual void hide() { this->setVisible(false); };

};

struct floatElement {
	std::string name;
	cBaseNode *node;
	cocos2d::Vec2 originPos;
	cDigit x, y;
	cDigit value;

	floatElement()
	{
		name.clear();
		node = NULL;
		originPos.setZero();
		x = 0;
		y = 0;
	};
	~floatElement()
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

#endif // !BASE_NODE_HPP
