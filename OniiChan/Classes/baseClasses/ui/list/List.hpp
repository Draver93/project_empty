#ifndef LIST_HPP
#define LIST_HPP

#include "cocos2d.h"
#include "baseClasses/BaseNode.hpp"
#include "baseClasses/ui/UI.hpp"


enum class ScrlVector
{
	HORIZONTAL,
	VERTICAL
};
class cList : public cBaseNode
{
private:
	bool bInit = false;
	ScrlVector vecType;
	cWindow *wnd;
	cocos2d::Node *field;

	cocos2d::Rect fieldRect;
	cocos2d::Rect borderRect;

	float indent;
	cocos2d::Vec2 pressedPos;
	cocos2d::Vec2 touchPos;

	void updatePos();
	std::vector<cocos2d::Size> pLineSizes;


	std::vector<std::vector<cBaseNode*>> pElements;
public:
	cBaseNode* pActiveElement;
	void updateVisible();

	cList(cocos2d::Size listSize, int indent, std::vector<cBaseNode*> elements, int width, ScrlVector falgs);
	~cList();
	std::vector<std::vector<cBaseNode*>> *getElements() { return &pElements; }
	cBaseNode *getActiveElement() { return pActiveElement; }
};

#endif // !LIST_HPP
