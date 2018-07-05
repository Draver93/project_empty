#include "DropDown.hpp"


cDropDown::cDropDown(cButton *button, std::vector<cBaseNode*> elements)
{
	this->autorelease();

	btn = button;
	currentTime = 2.5f;
	pElements = elements;

	this->addChild((cocos2d::Node*)btn);

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		elements.at(i)->setOpacity(0);
		this->addChild(elements.at(i));
	}

	isActive = false;
	for (unsigned int i = 0; i < pElements.size(); i++) pElements.at(i)->listener->setEnabled(false);
	animationTime = 2.5f;

	indent = 10;
	btn->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
	{
		currentTime = animationTime - currentTime;
		isActive = !isActive;
		return true;
	};
}
cDropDown::~cDropDown()
{
}

void cDropDown::updateState(bool state)
{
	float timeOne = animationTime / pElements.size();
	int pos = (int)(currentTime / timeOne);
	int alight = 1;
	if (state)
	{
		cocos2d::Vec2 btnPos = btn->getPosition();
		cocos2d::Rect btnRect = cocos2d::utils::getCascadeBoundingBox(btn);
		float ind = btnPos.y;
		ind -= btnRect.size.height / 2.0f;
		ind -= indent;

		for (int i = 0; i < pElements.size(); i++)
		{
			cocos2d::Rect elementRect = cocos2d::utils::getCascadeBoundingBox(pElements.at(i));
			ind -= elementRect.size.height / 2.0f;
			if (i < pos)
			{
				//
				cocos2d::Vec2 resultPos;
				resultPos = btnPos;

				pElements.at(i)->setOpacity(255);
				pElements.at(i)->setPosition(btnPos.x + elementRect.size.width / 2.0f * alight, ind);
			}
			else if (i == pos)
			{
				//min
				cocos2d::Vec2 minPos = btnPos;
				if (i > 0) minPos = pElements.at(i - 1)->getPosition();

				//max
				ind;
				float dDist = ind - minPos.y;
				float dTime = currentTime - (pos * timeOne);
				float tProc = dTime / (timeOne / 100.0f);
				float finalInd = dDist / 100.0f * tProc;
				//
				//получить мин макс их вычесть получить процент от времени и добавить к мин
				pElements.at(i)->setOpacity(255 / 100.0f * tProc);

				pElements.at(i)->setPosition(btnPos.x + elementRect.size.width / 2.0f * alight, minPos.y + finalInd);
			}
			else if (i > pos)
			{
				//
				pElements.at(i)->setPosition(pElements.at(pos)->getPosition());
				pElements.at(i)->setOpacity(0);
			}
			ind -= elementRect.size.height / 2.0f;
			ind -= indent;
		}
	}
	else
	{
		cocos2d::Vec2 btnPos = btn->getPosition();
		cocos2d::Rect btnRect = cocos2d::utils::getCascadeBoundingBox(btn);

		pos = pElements.size() - pos - 1;

		float ind = btnPos.y;
		ind -= btnRect.size.height / 2.0f;
		ind -= indent;

		for ( int i = 0; i < pElements.size(); i++)
		{
			cocos2d::Rect elementRect = cocos2d::utils::getCascadeBoundingBox(pElements.at(i));
			ind -= elementRect.size.height / 2.0f;
			if (i > pos)
			{
				if(pos < 0)	pElements.at(i)->setPosition(btnPos);
				else pElements.at(i)->setPosition(pElements.at(pos)->getPosition());
				//
				pElements.at(i)->setOpacity(0);		
			}
			else if (i == pos)
			{
				//min
				cocos2d::Vec2 minPos = btnPos;
				if (i > 0) minPos = pElements.at(i - 1)->getPosition();

				//max
				ind;
				float dDist = ind - minPos.y;
				float dTime = currentTime - ((pElements.size() - (pos + 1)) * timeOne);
				float tProc = dTime / (timeOne / 100.0f);
				float finalInd = dDist / 100.0f * tProc;
				//
				//получить мин макс их вычесть получить процент от времени и добавить к мин
				pElements.at(i)->setOpacity(255 - (255 / 100.0f * tProc));

				pElements.at(i)->setPosition(btnPos.x + elementRect.size.width / 2.0f * alight, ind - finalInd);
			}
			else if (i > pos)
			{
				//
				cocos2d::Vec2 resultPos;
				resultPos = btnPos;

				pElements.at(i)->setOpacity(255);
				//pElements.at(i)->setPosition(btnPos.x, ind);
			}

			ind -= elementRect.size.height / 2.0f;
			ind -= indent;
		}
	}
}
void cDropDown::update(float dt)
{
	if (currentTime < animationTime)
	{
		currentTime += dt;

		if (isActive)
		{
			updateState(true);
			for (unsigned int i = 0; i < pElements.size(); i++) pElements.at(i)->listener->setEnabled(true);

		}
		else
		{
			updateState(false);
			for (unsigned int i = 0; i < pElements.size(); i++) pElements.at(i)->listener->setEnabled(false);
		}

	}
	else currentTime = animationTime;
	float a = btn->getOpacity();
	bool b = btn->isVisible();
	cocos2d::Vec2 c = btn->getPosition();

}
