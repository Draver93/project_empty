#ifndef DROPDOWN_HPP
#define DROPDOWN_HPP

#include "cocos2d.h"
#include "baseClasses/BaseScene.hpp"
#include "baseClasses/ui/window/Window.hpp"
#include "baseClasses/ui/button/Button.hpp"

class cDropDown : public cocos2d::Node
{
private:
	float indent;
	bool isActive;
	float animationTime;
	float currentTime;

	cButton *btn;
	std::vector<cBaseNode*> pElements;

	void updateState(bool state);
public:
	cDropDown(cButton *button, std::vector<cBaseNode*> elements);
	~cDropDown();

	void update(float dt);
};

#endif // !DROPDOWN_HPP
