#include "List.hpp"

void cList::updatePos()
{
	float lineHeight = 0;
	float prevLineH = 0;
	for (int i = 0; i < pElements.size(); i++)
	{
		for (int j = 0; j < pElements.at(i).size(); j++)
		{
			cocos2d::Rect elRect = cocos2d::utils::getCascadeBoundingBox(pElements.at(i).at(j));

			if (elRect.size.height > lineHeight)lineHeight = elRect.size.height;

			float prevPos = 0;
			if (i - 1 > -1) prevPos = pElements.at(i - 1).at(0)->getPosition().y;

			if (j - 1 < 0) pElements.at(i).at(j)->setPosition(elRect.size.width / 2.0f + indent, prevPos - (prevLineH / 2.0f) - (elRect.size.height / 2.0f) - indent);
			else
			{
				cocos2d::Rect prevElRect = cocos2d::utils::getCascadeBoundingBox(pElements.at(i).at(j - 1));
				pElements.at(i).at(j)->setPosition(pElements.at(i).at(j - 1)->getPosition().x + prevElRect.size.width / 2.0f + elRect.size.width / 2.0f + indent, pElements.at(i).at(j - 1)->getPosition().y);
			}
		}
		prevLineH = lineHeight;
		lineHeight = 0;
	}

	pLineSizes.clear();
	for (int i = 0; i < pElements.size(); i++)
	{
		cocos2d::Size sz = { 0,0 };
		for (int j = 0; j < pElements.at(i).size(); j++)
		{
			cocos2d::Rect elRect = cocos2d::utils::getCascadeBoundingBox(pElements.at(i).at(j));
			if (sz.height < elRect.size.height)
				sz.height = elRect.size.height;
			if (sz.width < pElements.at(i).at(j)->getPosition().x + elRect.size.width / 2.0f)
				sz.width = pElements.at(i).at(j)->getPosition().x + elRect.size.width / 2.0f;
		}
		pLineSizes.push_back(sz);
	}

}

void cList::updateVisible()
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	borderRect = cocos2d::utils::getCascadeBoundingBox(wnd);
	for (unsigned int i = 0; i < pElements.size(); i++)
		for (unsigned int j = 0; j < pElements.at(i).size(); j++)
		{
			cocos2d::Rect elRect = cocos2d::utils::getCascadeBoundingBox(pElements.at(i).at(j));

			pElements.at(i).at(j)->setVisible(true);
			if (elRect.origin.x + elRect.size.width < borderRect.origin.x)
			{
				pElements.at(i).at(j)->setVisible(false);
			}
			else if (elRect.origin.x > borderRect.origin.x + borderRect.size.width)
			{
				pElements.at(i).at(j)->setVisible(false);
			}

			if (elRect.origin.y + elRect.size.height < borderRect.origin.y)
			{
				pElements.at(i).at(j)->setVisible(false);
			}
			else if (elRect.origin.y > borderRect.origin.y + borderRect.size.height)
			{
				pElements.at(i).at(j)->setVisible(false);
			}
		}
}

cList::cList(cocos2d::Size listSize, int indentSize, std::vector<cBaseNode*> elements, int width, ScrlVector falgs)
{
	pActiveElement = nullptr;
	indent = indentSize;
	field = cocos2d::Node::create();
	wnd = new cWindow(listSize, W_MUTE, 0);
	wnd->setOpacity(0);
	cocos2d::ClippingNode *node = cocos2d::ClippingNode::create();

	node->setStencil(wnd);	//set the cut triangle in the ClippingNode.
	node->setInverted(false);

	this->addChild(wnd);
	this->addChild(node);
	node->addChild(field);

	vecType = falgs;
	int lineWidth = elements.size() / width;
	pElements.resize(lineWidth + 1);

	int line = -1;
	for (int i = 0; i < elements.size(); i++)
	{
		field->addChild(elements.at(i));
		if (i % width == 0)line++;
		pElements.at(line).push_back(elements.at(i));
	}
	updatePos();
	field->setPosition(-listSize.width /2.0f, listSize.height / 2.0f);
	field->setScale(1.0f);

	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if (this->getParent() != nullptr && bInit == false)
		{
			fieldRect = cocos2d::utils::getCascadeBoundingBox(field);
			fieldRect.size.width += indent * field->getScale() * 2.0f;
			fieldRect.size.height += indent * field->getScale() * 2.0f;
			borderRect = cocos2d::utils::getCascadeBoundingBox(wnd);
			bInit = true;
		}
		cocos2d::Vec2 p = touch->getLocation();
		if (borderRect.containsPoint(p))
		{
			pressedPos = field->getPosition() - field->getParent()->convertTouchToNodeSpace(touch);
			touchPos = touch->getLocation(); //отвечает за место нажатия глобальное
			return true;
		}
		return false;
	};
	listener->onTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{		
		cocos2d::Vec2 p = this->getParent()->convertTouchToNodeSpace(touch);

		cocos2d::Vec2 newPos = { p.x + pressedPos.x, p.y + pressedPos.y };
		cocos2d::Vec2 dPos = newPos - field->getPosition();

		cocos2d::Vec2 finalShift = field->getPosition();
		if (dPos.x > 0 && borderRect.size.width < fieldRect.size.width)
		{
			if (newPos.x < -borderRect.size.width / 2.0f)
			{
				finalShift.x = newPos.x;
			}
			else finalShift.x = -borderRect.size.width / 2.0f;
		}
		else if (dPos.x < 0 && borderRect.size.width < fieldRect.size.width)
		{
			if (newPos.x + fieldRect.size.width > borderRect.size.width / 2.0f)
			{
				finalShift.x = newPos.x;
			}
			else 
				finalShift.x = borderRect.size.width / 2.0f - fieldRect.size.width;

		}
		if (dPos.y > 0 && borderRect.size.height < fieldRect.size.height)
		{
			if (newPos.y - fieldRect.size.height < -borderRect.size.height / 2.0f)
			{
				finalShift.y = newPos.y;
			}
			else finalShift.y = -borderRect.size.height / 2.0f + fieldRect.size.height;

		}
		else if (dPos.y < 0 && borderRect.size.height < fieldRect.size.height)
		{
			if (newPos.y > borderRect.size.height / 2.0f)
			{
				finalShift.y = newPos.y;
			}
			else finalShift.y = borderRect.size.height / 2.0f;
		}
		
		field->setPosition(finalShift);
	};
	listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		updateVisible();

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::Vec2 p = touch->getLocation();
		borderRect = cocos2d::utils::getCascadeBoundingBox(wnd);
		for (unsigned int i = 0; i < pElements.size(); i++)
			for (unsigned int j = 0; j < pElements.at(i).size(); j++)
			{
				cocos2d::Rect elRect = cocos2d::utils::getCascadeBoundingBox(pElements.at(i).at(j));
				if (abs(p.x - touchPos.x) < visibleSize.height / 100.0f && abs(p.y - touchPos.y) < visibleSize.height / 100.0f && elRect.containsPoint(p))
				{
					if (pActiveElement == pElements.at(i).at(j)) pActiveElement = nullptr;
					else pActiveElement = pElements.at(i).at(j);
				}

			}

	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 50);
}

cList::~cList()
{
}
