#include "Window.hpp"


cWindow::cWindow(cocos2d::Size nSize, unsigned int nFlags, int priority)
{
	init(nSize, nFlags, priority);
}
cWindow::cWindow()
{
}
cWindow::~cWindow()
{
}

void cWindow::update()
{
	cocos2d::Sprite *pSprite = (cocos2d::Sprite*)this->getChildByName("M_PANEL");
	pSprite->setContentSize(sMiddle);
	//Angle
	{
		pSprite = (cocos2d::Sprite*)this->getChildByName("LT_ANGLE");
		setNodeSize(pSprite, sBorder, false, true);
		cocos2d::Vec2 sSize = pSprite->getContentSize() * pSprite->getScale();
		pSprite->setPosition(cocos2d::Vec2(-sMiddle.width / 2.0f - sSize.x / 2.0f, sMiddle.height / 2.0f + sSize.y / 2.0f));

		pSprite = (cocos2d::Sprite*)this->getChildByName("LB_ANGLE");
		pSprite->setRotation(270);
		setNodeSize(pSprite, sBorder, false, true);
		sSize = pSprite->getContentSize() * pSprite->getScale();
		pSprite->setPosition(cocos2d::Vec2(-sMiddle.width / 2.0f - sSize.x / 2.0f, -sMiddle.height / 2.0f - sSize.y / 2.0f));

		pSprite = (cocos2d::Sprite*)this->getChildByName("RT_ANGLE");
		pSprite->setRotation(90);
		setNodeSize(pSprite, sBorder, false, true);
		sSize = pSprite->getContentSize() * pSprite->getScale();
		pSprite->setPosition(cocos2d::Vec2(sMiddle.width / 2.0f + sSize.x / 2.0f, sMiddle.height / 2.0f + sSize.y / 2.0f));

		pSprite = (cocos2d::Sprite*)this->getChildByName("RB_ANGLE");
		pSprite->setRotation(180);
		setNodeSize(pSprite, sBorder, false, true);
		sSize = pSprite->getContentSize() * pSprite->getScale();
		pSprite->setPosition(cocos2d::Vec2(sMiddle.width / 2.0f + sSize.x / 2.0f, -sMiddle.height / 2.0f - sSize.y / 2.0f));
	}

	//Border
	{
		pSprite = (cocos2d::Sprite*)this->getChildByName("T_PANEL");
		setNodeSize(pSprite, sBorder, false, false);
		cocos2d::Vec2 sSize = pSprite->getContentSize() * pSprite->getScale();
		pSprite->setContentSize(cocos2d::Size(sMiddle.width, sSize.y));
		pSprite->setPosition(cocos2d::Vec2(0, sMiddle.height / 2.0f + sSize.y / 2.0f));

		pSprite = (cocos2d::Sprite*)this->getChildByName("B_PANEL");
		pSprite->setRotation(180);
		setNodeSize(pSprite, sBorder, false, false);
		sSize = pSprite->getContentSize() * pSprite->getScale();
		pSprite->setContentSize(cocos2d::Size(sMiddle.width, sSize.y));
		pSprite->setPosition(cocos2d::Vec2(0, -sMiddle.height / 2.0f - sSize.y / 2.0f));

		pSprite = (cocos2d::Sprite*)this->getChildByName("R_PANEL");
		pSprite->setRotation(90);
		setNodeSize(pSprite, sBorder, false, false);
		sSize = pSprite->getContentSize() * pSprite->getScale();
		pSprite->setContentSize(cocos2d::Size(sMiddle.height, sSize.y));
		pSprite->setPosition(cocos2d::Vec2(sMiddle.width / 2.0f + sSize.y / 2.0f, 0));

		pSprite = (cocos2d::Sprite*)this->getChildByName("L_PANEL");
		pSprite->setRotation(-90);
		setNodeSize(pSprite, sBorder, false, false);
		sSize = pSprite->getContentSize() * pSprite->getScale();
		pSprite->setContentSize(cocos2d::Size(sMiddle.height, sSize.y));
		pSprite->setPosition(cocos2d::Vec2(-sMiddle.width / 2.0f - sSize.y / 2.0f, 0));
	}

}
void cWindow::init(cocos2d::Size nSize, unsigned int nFlags, int priority)
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	this->setCascadeOpacityEnabled(true);

	wndTouchBegin = wndTouchMoved = wndTouchEnded = nullptr;

	pressedNode = nullptr;
	sBorder = visibleSize.width > visibleSize.height ? visibleSize.height : visibleSize.width;
	sBorder /= 40.0f;
	flags = nFlags;
	sMiddle = nSize;
	sMiddle.width -= sBorder * 2.0f;
	sMiddle.height -= sBorder * 2.0f;
	if (sMiddle.width < 0) sMiddle.width = 0;
	if (sMiddle.height < 0) sMiddle.height = 0;

	std::vector<std::string> names = { "LT_ANGLE", "LB_ANGLE", "RT_ANGLE",
		"RB_ANGLE", "L_PANEL", "R_PANEL",
		"T_PANEL", "B_PANEL", "M_PANEL" };
	for (unsigned int i = 0; i < names.size(); i++)
	{
		cocos2d::Sprite *tmpSprite = nullptr;

		if (i < 4) tmpSprite = cocos2d::Sprite::create("button/module/angleBtn.png");
		else tmpSprite = cocos2d::Sprite::create("button/module/fillBtn.png");
		tmpSprite->setName(names.at(i));
		tmpSprite->setCascadeOpacityEnabled(true);
		if (tmpSprite != nullptr) this->addChild(tmpSprite);
	}

	////
	if (flags & W_MUTE) {}
	else
	{
		listener = cocos2d::EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
		{
			cocos2d::Vec2 p = touch->getLocation();
			auto childrens = this->getChildren();
			for (auto it : childrens)
			{
				cocos2d::Rect rect = it->getBoundingBox();
				rect.origin = this->convertToWorldSpace(rect.origin);
				if (rect.containsPoint(p))
				{
					pressedNode = it;
					pressedPos = p - this->getPosition();
					if (wndTouchBegin != nullptr) wndTouchBegin(touch, it);
					this->onTouchBegan(touch, it);
					return true;
				}
			}
			return false;
		};
		listener->onTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Event* event)
		{
			if (wndTouchMoved != nullptr) wndTouchMoved(touch, pressedNode);
			this->onTouchMoved(touch, pressedNode);
		};
		listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
		{
			if (wndTouchEnded != nullptr) wndTouchEnded(touch, pressedNode);
			this->onTouchEnded(touch, pressedNode);
		};

		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, priority);
	}
	update();
}
void cWindow::transform(std::string name, cocos2d::Touch * pTouch)
{
	if (name == "L_PANEL" && flags & W_SCALABLE)
	{
		cocos2d::Vec2 p = pTouch->getLocation();
		cocos2d::Vec2 pos = this->getPosition();
		p = this->convertToNodeSpace(p);
		float shiftX = (sMiddle.width / 2.0f) + p.x;
		this->setPosition(this->getPosition().x + shiftX / 2.0f, this->getPosition().y);
		sMiddle.width -= shiftX;
	}
	else if (name == "R_PANEL" && flags & W_SCALABLE)
	{
		cocos2d::Vec2 p = pTouch->getLocation();
		cocos2d::Vec2 pos = this->getPosition();
		p = this->convertToNodeSpace(p);
		float shiftX = (sMiddle.width / 2.0f) - p.x;
		this->setPosition(this->getPosition().x - shiftX / 2.0f, this->getPosition().y);
		sMiddle.width -= shiftX;
	}
	else if (name == "B_PANEL" && flags & W_SCALABLE)
	{
		cocos2d::Vec2 p = pTouch->getLocation();
		cocos2d::Vec2 pos = this->getPosition();
		p = this->convertToNodeSpace(p);
		float shiftY = (sMiddle.height / 2.0f) + p.y;
		this->setPosition(this->getPosition().x, this->getPosition().y + shiftY / 2.0f);
		sMiddle.height -= shiftY;
	}
	else if (name == "T_PANEL" && flags & W_MOVABLE)
	{
		cocos2d::Vec2 p = pTouch->getLocation();
		this->setPosition(p.x - pressedPos.x, p.y - pressedPos.y);
	}
}

void cWindow::setWColor(cocos2d::Color3B color)
{
	std::vector<std::string> names = { "LT_ANGLE", "LB_ANGLE", "RT_ANGLE",
										"RB_ANGLE", "L_PANEL", "R_PANEL",
										"T_PANEL", "B_PANEL", "M_PANEL" };
	for (unsigned int i = 0; i < names.size(); i++)
	{
		this->getChildByName(names.at(i))->setColor(color);
	}
}
cocos2d::Color3B cWindow::getWColor()
{
	return this->getChildByName("M_PANEL")->getColor();
}

void cWindow::onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
	update();
}
void cWindow::onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
	if (pressedNode != nullptr && (pressedNode->getName() == "L_PANEL" || pressedNode->getName() == "R_PANEL" ||
		pressedNode->getName() == "T_PANEL" || pressedNode->getName() == "B_PANEL"))
	{
		transform(pressedNode->getName(), pTouch);
		update();
	}
	else
	{
		cocos2d::Vec2 p = pTouch->getLocation();
		auto childrens = this->getChildren();
		for (auto it : childrens)
		{
			cocos2d::Rect rect = it->getBoundingBox();
			rect.origin = this->convertToWorldSpace(rect.origin);
			if (rect.containsPoint(p))
			{
				pressedNode = it;
				update();
				pressedPos = p - this->getPosition();
			}
		}
	}
}
void cWindow::onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
	update();
	pressedNode = nullptr;
}
