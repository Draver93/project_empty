#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "cocos2d.h"
#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseNode.hpp"

enum WindowState
{ 
	W_SCALABLE = 1, 
	W_MOVABLE = 1 << 1,
	W_MUTE = 1 << 2
};

class cWindow : public cBaseNode
{
protected:
	void init(cocos2d::Size size, unsigned int falgs, int priority);
private:
	cocos2d::Size sMiddle;
	float sBorder;
	unsigned int flags;
	cocos2d::Node* pressedNode;
	cocos2d::Vec2 pressedPos;
	void transform(std::string name, cocos2d::Touch * pTouch);
public:
	std::function<bool(cocos2d::Touch *, cocos2d::Node*)>wndTouchBegin;
	std::function<bool(cocos2d::Touch *, cocos2d::Node*)>wndTouchMoved;
	std::function<bool(cocos2d::Touch *, cocos2d::Node*)>wndTouchEnded;

	cWindow();
	~cWindow();

	cWindow(cocos2d::Size nSize, unsigned int nFlags, int priority);
	void update();

	void setWSize(cocos2d::Size nSize) 
	{ 
		sMiddle = cocos2d::Size(nSize.width - sBorder * 2.0f, nSize.height - sBorder * 2.0f); 
		if (sMiddle.width < 0) sMiddle.width = 0;
		if (sMiddle.height < 0) sMiddle.height = 0;
		update();
	}
	cocos2d::Size getWSize() { return cocos2d::Size(sMiddle.width + sBorder * 2.0f, sMiddle.height + sBorder * 2.0f); }

	void setWColor(cocos2d::Color3B color);
	cocos2d::Color3B getWColor();

	void onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Node *pNode);
};
#endif //! WINDOW_HPP