#include "BaseCards.hpp"


cCard::cCard()
{
	name = "";
	filePath = "";
	coment = "";
	subject = -1;
	stat = -1;
	rare = -1;
}
cCard::~cCard()
{
}

cocos2d::Node * cCard::getCardFront(cocos2d::TTFConfig * gTtfConfig, float width, std::string name, std::string path, int subject, float stat, int rare)
{
	cocos2d::Sprite *base = cocos2d::Sprite::create("cards/btm.png");
	base->setCascadeOpacityEnabled(true);

	cocos2d::Sprite *avatar = cocos2d::Sprite::create(path);
	cocos2d::Size avaSize = { base->getContentSize().width , base->getContentSize().height * 0.8f };
	setNodeInRect(avatar, avaSize, avaSize.width * 0.1f);
	avaSize = avatar->getContentSize() * avatar->getScale();
	avatar->setPosition(base->getContentSize().width / 2.0f, avaSize.height / 2.0f + base->getContentSize().height * 0.1f);
	base->addChild(avatar);

	cocos2d::Sprite *front = nullptr;
	switch (rare)
	{
	case 0: front = cocos2d::Sprite::create("cards/bronze_f.png");
		break;
	case 1: front = cocos2d::Sprite::create("cards/silver_f.png");
		break;
	case 2: front = cocos2d::Sprite::create("cards/gold_f.png");
		break;
	case 3: front = cocos2d::Sprite::create("cards/blue_f.png");
		break;
	}

	front->setPosition(base->getContentSize() / 2.0f);
	base->addChild(front);

	setNodeSize(base, width, true, true);
	return base;
}

cocos2d::Node * cCard::getCardBack(int rare, float width)
{
	cocos2d::Sprite *sprite = nullptr;
	switch (rare)
	{
	case 0: sprite = cocos2d::Sprite::create("cards/bronze_b.png");
		break;
	case 1: sprite = cocos2d::Sprite::create("cards/silver_b.png");
		break;
	case 2: sprite = cocos2d::Sprite::create("cards/gold_b.png");
		break;
	case 3: sprite = cocos2d::Sprite::create("cards/blue_b.png");
		break;
	}
	setNodeSize(sprite, width, true, true);
	return sprite;
}
