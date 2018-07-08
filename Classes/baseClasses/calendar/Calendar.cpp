#include "Calendar.hpp"

void cCalendar::updateList()
{
	float numEl = (size.height / (elemSize.height + indent));

	float min = -size.height / 2.0f + (elemSize.height * 1.5) + indent;
	float max = (list.size() - 3) * (elemSize.height + indent);
	if (listPos.get() < min)
	{
		listPos.setDirect(min);
	}
	else if(listPos.get() > max)
	{
		listPos.setDirect(max);
	}

	int first = listPos.get() / (elemSize.height + indent);
	int last = (listPos.get() + size.height) / (elemSize.height + indent);

	if (currentPos < first) 
	{
		calElement *fst = wnds.front();
		wnds.erase(wnds.begin());
		wnds.push_back(fst);
		currentPos = first;		
	}
	else  if (currentPos > first)
	{
		calElement *lst = wnds.back();
		wnds.pop_back();
		wnds.insert(wnds.begin(), lst);
	}
	currentPos = first;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	for (int i = 0; i < (int)wnds.size(); i++)
	{
		if ((first + i) < 0 || (first + i) > list.size() - 1)
		{
			wnds.at(i)->wnd->setPosition(0, visibleSize.height + elemSize.height);
			wnds.at(i)->originPos = cocos2d::Vec2(0, visibleSize.height + elemSize.height);
			continue;
		}

		cocos2d::Size posEl = { 0, (-(((first + i) * (elemSize.height + indent)) - listPos.get()) + size.height / 2.0f) - (elemSize.height * 1.5f + indent) };
		wnds.at(i)->wnd->setPosition(posEl);
		wnds.at(i)->originPos = posEl;
		//
		float scaleBorder = size.height / 5.0f;
		if (abs(posEl.height) > scaleBorder)
		{
			float dx = abs(posEl.height) - scaleBorder;
			float dS = size.height / 2.0f - scaleBorder;
			float proc = 100.0f - dx / (dS / 100.0f) ;
			if (proc < 0)proc = 0; else if (proc > 100)proc = 100;

			wnds.at(i)->wnd->setOpacity((255.0f / 100.0f) * proc);
			wnds.at(i)->wnd->setScale(0.6f + (0.4f / 100.0f) * proc);
		}
		else
		{
			wnds.at(i)->wnd->setOpacity(255.0f);
			wnds.at(i)->wnd->setScale(1.0f);
		}
		//

		wnds.at(i)->day->setString(list.at(first + i).day);
		setNodeInRect(wnds.at(i)->day, daySz, 0);
		wnds.at(i)->date->setString(list.at(first + i).date);
		setNodeInRect(wnds.at(i)->date, dateSz, 0);

		if (list.at(first + i).mark == -1)
		{
			wnds.at(i)->label->setString(u8"?????????");
			setNodeInRect(wnds.at(i)->label, textSize, 0);
			wnds.at(i)->markerWnd->setWColor(cocos2d::Color3B(150, 150, 150));

			continue;
		}

		wnds.at(i)->label->setString(list.at(first + i).text);
		setNodeInRect(wnds.at(i)->label, textSize, 0);

		int mk = list.at(first + i).mark;
		if (mk == 5 || mk == 1)
		{
			//green
			wnds.at(i)->markerWnd->setWColor(cocos2d::Color3B(35, 110, 35));
		}
		else if (mk == 4)
		{
			//lite green
			wnds.at(i)->markerWnd->setWColor(cocos2d::Color3B(100, 190, 21));
		}
		else if (mk == 3)
		{
			// orange
			wnds.at(i)->markerWnd->setWColor(cocos2d::Color3B(250, 190, 50));
		}
		else if (mk == 2 || mk == 0)
		{
			// red
			wnds.at(i)->markerWnd->setWColor(cocos2d::Color3B(230, 70, 10));
		}
	}
}

cCalendar::cCalendar(cocos2d::Size dSize)
{
	isPress = false;
	size = dSize;
	lastListPos = 0;
	for (int i = 0; i < 5; i++)time_points[i] = 0;

	currentPos = 0;
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;
	elemSize = cocos2d::Size{ dSize.width * 0.8f, visibleSize.height / 12.0f };
	indent = elemSize.height * 0.4f;
	auto history = userDefault->history;
	for (unsigned int scId = 0; scId < userDefault->schools.size(); scId++)
	{
		sSchool sc = userDefault->schools.at(scId);
		for (unsigned int clId = 0; clId < sc.classes.size(); clId++)
		{
			sClass cl = sc.classes.at(clId);
			for (unsigned int semId = 0; semId < cl.semesters.size(); semId++)
			{
				sSem sem = cl.semesters.at(semId);
				for (unsigned int dayId = 0; dayId < sem.days.size(); dayId++)
				{
					dayLevel el;
					el.date = std::to_string(scId + 1) + ":" + std::to_string(clId + 1) + ":" + std::to_string(semId + 1);
					el.day = std::to_string(dayId + 1);

					std::string key = std::to_string(scId + 1) + "|" + std::to_string(clId + 1) + "|" + std::to_string(semId + 1) + "|" + std::to_string(dayId + 1);
					el.mark;
					if (history[key] != nullptr)
					{
						std::string val = history[key];
						auto data = Split(val, '|');
						el.text = u8"" + data[0];
						el.mark = std::stoi(data[1]);
					}
					list.push_back(el);
				}
			}
		}
	}

	int numEl = (dSize.height / (elemSize.height + indent));
	for (int i = 0; i < numEl; i++)
	{
		calElement* el = new calElement();
		el->originSize = elemSize;

		el->wnd = new cWindow(el->originSize, W_MUTE, 10);
		el->wnd->setName("el_" + std::to_string(i));
		el->wnd->setPosition(0, -dSize.height / 2.0f + indent + elemSize.height / 2.0f + (indent + elemSize.height) * i);
		el->wnd->setOpacity(200);
		this->addChild(el->wnd);
		wnds.push_back(el);


		float locIndent = elemSize.height * 0.05f;
		cocos2d::Size sz = { elemSize.width * 0.2f , elemSize.height - locIndent * 4.0f };
		el->markerWnd = new cWindow(sz, W_MUTE, 10);
		el->wnd->addChild(el->markerWnd);
		el->markerWnd->setPosition(el->originSize.width / 2.0f - locIndent * 2.0f - sz.width / 2.0f, 0);
		el->markerWnd->setWColor(cocos2d::Color3B(150, 150, 150));

		textSize = cocos2d::Size(elemSize.width * 0.4f, elemSize.height * 0.7f);
		el->label = cocos2d::Label::createWithTTF(*gTtfConfig, "?????????", cocos2d::TextHAlignment::CENTER);
		setNodeInRect(el->label, textSize, 0);
		el->label->setName("text");
		el->label->setColor(cocos2d::Color3B(150, 150, 150));
		el->wnd->addChild(el->label);

		daySz = cocos2d::Size(elemSize.height - locIndent * 2.0f, (elemSize.height - locIndent * 3.0f) * 0.8f);
		el->day = cocos2d::Label::createWithTTF(*gTtfConfig, "?", cocos2d::TextHAlignment::CENTER);
		el->day->setColor(cocos2d::Color3B(96, 139, 171));
		el->day->setName("day");
		setNodeInRect(el->day, daySz, 0);
		el->wnd->addChild(el->day);
		el->day->setPosition(-el->originSize.width / 2.0f + locIndent + daySz.width / 2.0f, el->originSize.height / 2.0f - locIndent - daySz.height / 2.0f);

		dateSz = cocos2d::Size(daySz.width * 1.5f, (elemSize.height - locIndent * 3.0f) * 0.2f);
		el->date = cocos2d::Label::createWithTTF(*gTtfConfig, "?????", cocos2d::TextHAlignment::CENTER);
		el->date->setColor(cocos2d::Color3B(96, 139, 171));
		el->date->setName("date");
		setNodeInRect(el->date, dateSz, 0);
		el->wnd->addChild(el->date);
		el->date->setPosition(-el->originSize.width / 2.0f + locIndent + daySz.width / 2.0f, el->day->getPosition().y - daySz.height /2.0f - locIndent / 2.0f - dateSz.height / 2.0f);

	}


	this->listener = cocos2d::EventListenerTouchOneByOne::create();
	this->listener->setSwallowTouches(true);
	this->listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		p = this->convertToNodeSpace(p);
		cocos2d::Rect rect;
		rect.size = size;
		rect.origin = (size / 2.0f) * -1;

		if (rect.containsPoint(p))
		{
			isPress = true;
			updateList();
			pickPoint = p;
			pickPos = listPos.get();
			lastListPos = listPos.get();
			return true;
		}
		return false;
	};
	this->listener->onTouchMoved = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		p = this->convertToNodeSpace(p);

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto dS = p - pickPoint;

		listPos.set(pickPos + dS.y);
		updateList();

		return true;
	};
	this->listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		isPress = false;
		float dP = listPos.get() - lastListPos;
		if (dP != 0)
		{
			listPos.set(listPos.get() + dP * 10.0f, 2.0f, true);
		}
		return true;
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->listener, 1);

	this->secondUpdate = [&](float dt)
	{
		addPos.update(dt);
		listPos.update(dt);
		if (listPos.isProgress()) updateList();

		//closer
		if (!isPress) time_points[0] += dt;
		else time_points[0] = 0;

		time_points[1] += dt;

		float firstPos = size.height / 2.0f - indent - (elemSize.height / 2.0f) * 1.5f;
		if (time_points[0] > 1.1f && !listPos.isProgress() && abs(firstPos - wnds.at(0)->originPos.y) > 0.01f)
		{
			updateList();
			listPos.set(listPos.get() + (firstPos - wnds.at(0)->originPos.y), 0.2f);
		}

		//inertia
		if (time_points[1] > 0.1f)
		{
			lastListPos = listPos.get();
			time_points[1] = 0;
		}
		for (auto it : wnds)it->update(dt, indent / 2.0f);
	};

	updateList();
}
cCalendar::~cCalendar()
{
}

