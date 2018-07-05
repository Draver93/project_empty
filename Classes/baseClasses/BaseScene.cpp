#include "BaseScene.hpp"

cBaseScene::cBaseScene()
{
	state = SceneState::ACTIVE;
	/*fadeTime = 0.5f;
	fade = cocos2d::LayerColor::create(cocos2d::Color4B(255, 255, 255, 0));
	fade->setLocalZOrder(100);
	this->addChild(fade);
	this->retain();*/
}

cBaseScene::~cBaseScene()
{
}
