#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "cocos2d.h"
#include "baseClasses/BaseScene.hpp"
#include "baseClasses/ui/window/Window.hpp"
#include "baseClasses/BaseNode.hpp"

class cSlider : public cBaseNode
{
private:
	cocos2d::Node* pressedNode;
	cocos2d::Vec2 pressedPos;
	cocos2d::Size bgSize;
	float fLenght;

public:
	cSlider(float lenght, cocos2d::Size size, int priority);
	~cSlider();

	void update();

	void onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Node *pNode);

	void setValue(float nValue);
	float getValue();

};

#endif // !SLIDER_HPP
