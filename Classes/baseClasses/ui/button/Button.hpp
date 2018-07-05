#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "cocos2d.h"

#include "vector"
#include "string"
#include "baseClasses/ui/window/Window.hpp"


class cButton : public cBaseNode
{
private:
	float scale;
	cocos2d::Size origin;
public:
	cWindow *wnd;
	std::function<bool(cocos2d::Touch *, cocos2d::Node*)>btnDown;
	std::function<bool(cocos2d::Touch *, cocos2d::Node*)>btnUp;

	cButton(cocos2d::Node *pNode, cocos2d::Size size, float btnScale, int priority = 10);
	~cButton();

	void update();

	void onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
};

#endif // !BUTTON_HPP
