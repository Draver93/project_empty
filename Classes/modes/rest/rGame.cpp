#include "rGame.hpp"

cRGame::cRGame(cocos2d::Size size, cBaseMode * thisMode)
{
	addtime = 0;
	turns = 0;
	elementCount = 23;
	auto userDefault = cUserDefault::getInstance();
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	float indent = visibleSize.height / 70.0f;
	float halfindent = indent / 2.0f;

	cocos2d::Size btmBarSize;
	if (size.width > size.height * 0.6f) btmBarSize = cocos2d::Size(size.height * 0.6f, size.height * 0.6f);
	else btmBarSize = cocos2d::Size(visibleSize.width - indent * 2.0f, size.width);

	cWindow *btmBar = new cWindow(btmBarSize, W_MUTE, 0);
	cocos2d::Rect btmBarRect = cocos2d::utils::getCascadeBoundingBox(btmBar);
	float barInd = (visibleSize.width - btmBarRect.size.width) / 2.0f;
	btmBar->setPosition(btmBarRect.size.width / 2.0f + barInd, btmBarRect.size.height / 2.0f + indent);
	btmBar->setWColor(cocos2d::Color3B(0, 0, 0));
	btmBar->setOpacity(100);
	btmBar->setLocalZOrder(2);
	this->addChild(btmBar);

	field = new cGameField(visibleSize.width, cocos2d::Vec2(8, 7));
	cocos2d::Size fSize = field->getContentSize() * field->getScale();
	btmBar->addChild((cocos2d::Node*)field);
	float a = (visibleSize.width - indent * 4.0f);
	float b = btmBarSize.width - indent * 2.0f;
	field->setScale((btmBarSize.width - indent * 2.0f) / visibleSize.width);
	field->setPosition(-btmBarSize.width / 2.0f + indent, -btmBarSize.height / 2.0f + indent);
	field->setLocalZOrder(3);
}

cRGame::~cRGame()
{
}

void cRGame::step(float dt)
{
	if (addtime > 0.2 && (int)field->getElements().size() < elementCount)
	{
		//int randNum = rand() % 4;
		//field->addElement(subjects.at(randNum));
		int randNum = (rand() % 8) + 1;
		field->addElement(randNum);
		addtime = 0;
	}
	addtime += dt;
}

void cRGame::reset()
{
}
