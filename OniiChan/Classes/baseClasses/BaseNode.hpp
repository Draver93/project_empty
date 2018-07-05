#ifndef BASE_NODE_HPP
#define BASE_NODE_HPP

#include "cocos2d.h"
#include "string"
#include "vector"

class cBaseNode : public cocos2d::Node
{
private:
public:
	cocos2d::EventListenerTouchOneByOne *listener;
	std::function<void(float)> secondUpdate;

	cBaseNode();
	~cBaseNode();

	void update(float dt) override;
};

#endif // !BASE_NODE_HPP
