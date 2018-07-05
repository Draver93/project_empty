#include "CheckBox.hpp"

cCheckBox::cCheckBox(cocos2d::Size size, int priority)
{
	this->setCascadeOpacityEnabled(true);
	isActive = false;
	bgSize = size;
	cWindow *background = new cWindow(size, W_MUTE, 0);
	background->setWColor(cocos2d::Color3B(70, 200, 70));
	background->setName("BACKGROUND");
	this->addChild(background);

	cWindow *btnSwitch = new cWindow(cocos2d::Size(size.width / 3.0f, size.height - size.width / 10.0f), W_MUTE, 0);
	btnSwitch->setWColor(cocos2d::Color3B(230, 230, 230));
	btnSwitch->setName("SWITCH");
	this->addChild(btnSwitch);

	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = this->convertToNodeSpace(touch->getLocation());

		cocos2d::Rect rectSW = cocos2d::utils::getCascadeBoundingBox(this->getChildByName("SWITCH"));
		rectSW.origin = this->convertToNodeSpace(rectSW.origin);

		if (rectSW.containsPoint(p))
		{
			pressedNode = this->getChildByName("SWITCH");
			pressedPos = p - pressedNode->getPosition();
			this->onTouchBegan(touch, pressedNode);
			return true;
		}	
		return false;
	};
	listener->onTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		this->onTouchMoved(touch, pressedNode);		
	};
	listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		this->onTouchEnded(touch, pressedNode);
		pressedNode = nullptr;
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, priority);

	setFinalPos();
}
cCheckBox::~cCheckBox()
{
}



void cCheckBox::setActive(bool state)
{
	if (state)
	{
		((cWindow*)this->getChildByName("BACKGROUND"))->setWColor(cocos2d::Color3B(70, 200, 70));
		isActive = true;
	}
	else
	{
		((cWindow*)this->getChildByName("BACKGROUND"))->setWColor(cocos2d::Color3B(247, 116, 98));
		isActive = false;
	}
}
bool cCheckBox::getActive()
{
	return isActive;
}

void cCheckBox::setFinalPos()
{
	float indent = bgSize.width / 20.0f;
	cocos2d::Node *nSwitch = this->getChildByName("SWITCH");
	cocos2d::Rect rectBG = cocos2d::utils::getCascadeBoundingBox(this->getChildByName("BACKGROUND"));
	cocos2d::Rect rectSW = cocos2d::utils::getCascadeBoundingBox(nSwitch);

	cocos2d::Vec2 p = nSwitch->getPosition();
	float right = rectBG.size.width / 2.0f - indent - rectSW.size.width / 2.0f;
	float left = -rectBG.size.width / 2.0f + indent + rectSW.size.width / 2.0f;

	if (p.x >= 0)
	{
		nSwitch->setPosition(right, nSwitch->getPosition().y);
		setActive(true);
	}
	else if (p.x <= 0)
	{
		nSwitch->setPosition(left, nSwitch->getPosition().y);
		setActive(false);
	}
}

void cCheckBox::onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
}
void cCheckBox::onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
	float indent = bgSize.width / 20.0f ;
	cocos2d::Rect rectBG = cocos2d::utils::getCascadeBoundingBox(this->getChildByName("BACKGROUND"));
	rectBG.origin = this->convertToNodeSpace(rectBG.origin);
	cocos2d::Rect rectSW = cocos2d::utils::getCascadeBoundingBox(pNode);
	rectSW.origin = this->convertToNodeSpace(rectSW.origin);

	cocos2d::Vec2 p = this->convertToNodeSpace(pTouch->getLocation());

	p.x -= pressedPos.x;
	float right = rectBG.size.width / 2.0f - indent - rectSW.size.width / 2.0f;
	float left = -rectBG.size.width / 2.0f + indent + rectSW.size.width / 2.0f;
	if (p.x > right)
	{
		pNode->setPosition(right, pNode->getPosition().y);
		setActive(true);
	} 
	else if (p.x < left)
	{
		pNode->setPosition(left, pNode->getPosition().y);
		setActive(false);
	}
	else
	{
		pNode->setPosition(p.x, pNode->getPosition().y);
	}
}
void cCheckBox::onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
	setFinalPos();
}


