#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseNode.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/UserDefault.hpp"
#include "baseClasses/digitEx/cDigit.hpp"


struct dayLevel
{
	std::string day;
	std::string date;
	std::string text;
	int mark;
	dayLevel()
	{
		day.clear();
		date.clear();
		text.clear();
		mark = -1;
	}
};
struct calElement
{
	cWindow *markerWnd;
	cocos2d::Label *label;
	cocos2d::Label *day;
	cocos2d::Label *date;
	cocos2d::Label *mark;

	cocos2d::Size originSize;
	cocos2d::Vec2 originPos;
	cDigit x, y;
	cWindow *wnd;
	~calElement()
	{
	};
	void update(float dt, float indent)
	{	
		x.update(dt);
		y.update(dt);
		if (!x.isProgress())
		{
			int proc = rand() % 100;
			float ind = (indent / 100.0f) * proc - indent / 2.0f;

			x.set(ind, (rand() % 4) + 1);
		}
		if (!y.isProgress())
		{
			int proc = rand() % 100;
			float ind = (indent / 100.0f) * proc - indent / 2.0f;

			y.set(ind, (rand() % 4) + 1);
		}
		wnd->setPosition(originPos.x + x.get(), originPos.y + y.get());
	};
};

class cCalendar : public cBaseNode
{
private:
	std::vector<dayLevel> list;
	int currentPos;
	std::vector<calElement*> wnds;

	cocos2d::Size size;
	cocos2d::Size textSize;
	cocos2d::Size elemSize;
	cocos2d::Size dateSz, daySz;

	float indent;
	cDigit listPos;
	float pickPos;
	cocos2d::Vec2 pickPoint;

	cDigit addPos;
	float lastListPos;

	float time_points[5];

	bool isPress;


	void updateList();
public:
	cCalendar(cocos2d::Size dSize);
	~cCalendar();
};

#endif // !CALENDAR_HPP
