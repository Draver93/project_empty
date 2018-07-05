#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "cocos2d.h"
#include "baseClasses/BaseScene.hpp"
#include "baseClasses/ui/window/Window.hpp"
#include "baseClasses/BaseNode.hpp"

class cCheckBox : public cBaseNode
{
private:
	cocos2d::Node* pressedNode;
	cocos2d::Vec2 pressedPos;

	cocos2d::Size bgSize;

	bool isActive;
public:
	cCheckBox(cocos2d::Size size, int priority);
	~cCheckBox();

	void setActive(bool state);
	bool getActive();
	void setFinalPos();
	void onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
};

#endif // !CHECKBOX_HPP
