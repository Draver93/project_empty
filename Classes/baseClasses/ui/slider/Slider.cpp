#include "Slider.hpp"

cSlider::cSlider(float lenght, cocos2d::Size size, int priority)
{
	bgSize = size;
	fLenght = lenght;
	this->setCascadeOpacityEnabled(true);
	cWindow *lineBg = new cWindow(size, W_MUTE, 0);
	lineBg->setWColor(cocos2d::Color3B(150, 150, 150));
	lineBg->setName("LBG");
	this->addChild(lineBg);

	cWindow *lineBgActive = new cWindow(size, W_MUTE, 0);
	lineBgActive->setWColor(cocos2d::Color3B(70, 200, 70));
	lineBgActive->setWSize(cocos2d::Size(20, size.height));
	lineBgActive->setName("LBGA");
	this->addChild(lineBgActive);

	cWindow *btnSlider = new cWindow(cocos2d::Size(size.height, size.height), W_MUTE, 0);
	btnSlider->setWColor(cocos2d::Color3B(230, 230, 230));
	btnSlider->setName("SLIDER");
	this->addChild(btnSlider);

	/*cWindow *valWnd = new cWindow(, W_MUTE, 0);
	valWnd->setWColor(cocos2d::Color3B(230, 230, 230));
	valWnd->setName("VAL");
	this->addChild(valWnd);*/

	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Node *pNode = this->getChildByName("SLIDER");
		cocos2d::Vec2 p = this->convertToNodeSpace(touch->getLocation());
		cocos2d::Rect rectSL = cocos2d::utils::getCascadeBoundingBox(pNode);
		rectSL.origin = this->convertToNodeSpace(rectSL.origin);

		if (rectSL.containsPoint(p))
		{
			pressedNode = pNode;
			pressedPos = p - pressedNode->getPosition();
			this->onTouchBegan(touch, pressedNode);
			update();
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

	update();
}
cSlider::~cSlider()
{
}

void cSlider::update()
{
	cWindow *lineBg = (cWindow *)this->getChildByName("LBG");
	cWindow *lineBgActive = (cWindow *)this->getChildByName("LBGA");
	cWindow *slider = (cWindow *)this->getChildByName("SLIDER");

	float rot = this->getRotation();
	float rad = rot * (3.14 / 180.0f);
	this->setRotation(0); //for bounding boxes
	cocos2d::Rect rectBG = cocos2d::utils::getCascadeBoundingBox(lineBg);
	cocos2d::Rect rectSL = cocos2d::utils::getCascadeBoundingBox(slider);

	lineBgActive->setWSize(cocos2d::Size(lineBg->getWSize().width / 2.0f + slider->getPosition().x, lineBgActive->getWSize().height));

	cocos2d::Rect rectBGA = cocos2d::utils::getCascadeBoundingBox(lineBgActive);
	lineBgActive->setPosition((lineBg->getPosition().x - rectBG.size.width / 2.0f) + (rectBGA.size.width / 2.0f),lineBgActive->getPosition().y);

	this->setRotation(rot);
}

void cSlider::onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
}
void cSlider::onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
	cWindow *lineBg = (cWindow *)this->getChildByName("LBG");
	cWindow *lineBgActive = (cWindow *)this->getChildByName("LBGA");

	float rot = this->getRotation();
	float rad = rot * (3.14 / 180.0f);
	this->setRotation(0); //for bounding boxes
	cocos2d::Rect rectBG = cocos2d::utils::getCascadeBoundingBox(lineBg);
	rectBG.origin = this->convertToNodeSpace(rectBG.origin);

	cocos2d::Rect rectSL = cocos2d::utils::getCascadeBoundingBox(pNode);
	rectSL.origin = this->convertToNodeSpace(rectSL.origin);

	cocos2d::Vec2 p = this->convertToNodeSpace(pTouch->getLocation()) - pressedPos;
	float right = rectBG.size.width / 2.0f - rectSL.size.width / 2.0f;
	float left = -rectBG.size.width / 2.0f + rectSL.size.width / 2.0f;
	float indent = p.x * cos(rad) + p.y * -sin(rad);

	if (indent > right)
	{
		pNode->setPosition(right, pNode->getPosition().y);
	}
	else if (indent < left)
	{
		pNode->setPosition(left, pNode->getPosition().y);
	}
	else
	{
		pNode->setPosition(indent, pNode->getPosition().y);
	}

	update();

	this->setRotation(rot);
}
void cSlider::onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Node * pNode)
{
}

void cSlider::setValue(float nValue)
{
	cWindow *lineBg = (cWindow *)this->getChildByName("LBG");
	cWindow *lineBgActive = (cWindow *)this->getChildByName("LBGA");
	cWindow *slider = (cWindow *)this->getChildByName("SLIDER");
	float rot = this->getRotation();
	float rad = rot * (3.14 / 180.0f);
	this->setRotation(0); //for bounding boxes
	cocos2d::Rect rectBG = cocos2d::utils::getCascadeBoundingBox(lineBg);
	cocos2d::Rect rectSL = cocos2d::utils::getCascadeBoundingBox(slider);

	float p = nValue;
	float right = rectBG.size.width / 2.0f - rectSL.size.width / 2.0f;
	float left = -rectBG.size.width / 2.0f + rectSL.size.width / 2.0f;

	if (p > right)
	{
		slider->setPosition(right, slider->getPosition().y);
	}
	else if (p < left)
	{
		slider->setPosition(left, slider->getPosition().y);
	}
	else
	{
		slider->setPosition(p, slider->getPosition().y);
	}

	update();
}
float cSlider::getValue()
{
	cWindow *lineBg = (cWindow *)this->getChildByName("LBG");
	cWindow *lineBgActive = (cWindow *)this->getChildByName("LBGA");
	cWindow *slider = (cWindow *)this->getChildByName("SLIDER");

	float rot = this->getRotation();
	this->setRotation(0); //for bounding boxes

	cocos2d::Rect rectBG = cocos2d::utils::getCascadeBoundingBox(lineBg);
	cocos2d::Rect rectSL= cocos2d::utils::getCascadeBoundingBox(slider);

	float right = rectBG.size.width / 2.0f - rectSL.size.width / 2.0f;
	float left = -rectBG.size.width / 2.0f + rectSL.size.width / 2.0f;
	right -= left;
	float slPos = slider->getPosition().x - left;
	if (slPos < 0.0001)slPos = 0;
	left = 0;

	float step = right / fLenght;

	this->setRotation(rot);
	return slPos / step;
}

