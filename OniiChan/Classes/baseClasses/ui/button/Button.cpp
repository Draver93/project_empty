#include "Button.hpp"

cButton::cButton(cocos2d::Node *pNode, cocos2d::Size size, float btnScale, int priority)
{
	this->setCascadeOpacityEnabled(true);
	btnUp = btnDown = nullptr;
	if (pNode != nullptr)
	{
		this->addChild(pNode);
		pNode->setLocalZOrder(this->getLocalZOrder() + 1);
	}
	origin = this->getContentSize();
	scale = btnScale;
	wnd = new cWindow(size, 0, priority);
	this->addChild(wnd);
	auto boundingBox = this->getBoundingBox();
	auto ch = this->getChildren();
	for (auto && child : ch)
	{
		cocos2d::Rect rc = child->getBoundingBox();
		boundingBox.unionWithRect(child->getBoundingBox());
	}
	this->listener = wnd->listener;
	wnd->wndTouchBegin = [=](cocos2d::Touch* touch, cocos2d::Node* node)
	{
		onTouchBegan(touch, node);
		return true;
	};
	wnd->wndTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Node* node)
	{
		onTouchMoved(touch, node);
		return true;
	};
	wnd->wndTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Node* node)
	{
		onTouchEnded(touch, node);
		return true;
	};
}
cButton::~cButton()
{
}

void cButton::update()
{
}
void cButton::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Node *pNode)
{
	if (btnDown != nullptr) btnDown(pTouch, pNode);
	this->setScale(scale);
}
void cButton::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Node *pNode)
{
}
void cButton::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Node *pNode)
{
	if (btnUp != nullptr) btnUp(pTouch, pNode);
	this->setScale(1);
}
