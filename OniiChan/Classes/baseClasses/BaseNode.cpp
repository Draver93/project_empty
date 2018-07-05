#include "BaseNode.hpp"

cBaseNode::cBaseNode()
{
	this->autorelease();
	this->scheduleUpdate();
	listener = nullptr;
	secondUpdate = nullptr;
}

cBaseNode::~cBaseNode()
{
	if (listener != nullptr)
	{
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
		listener = nullptr;
	}
	secondUpdate = nullptr;
}

void cBaseNode::update(float dt)
{
	cocos2d::Node *parent = this;
	if (listener != nullptr)
	{
		while (parent != nullptr)
		{
			if (!parent->isVisible())
			{
				listener->setEnabled(false); return;
			}
			parent = parent->getParent();
		}
		listener->setEnabled(true);
	}
	if (secondUpdate != nullptr && listener->isEnabled()) secondUpdate(dt);
}
