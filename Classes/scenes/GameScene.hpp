#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseScene.hpp"
#include "SceneManager.hpp"

#include "baseClasses/ui/UI.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/UserDefault.hpp"
#include "baseClasses/dialogs/Dialog.hpp"


class cGameScene : public cBaseScene
{
private:
	cocos2d::Sprite *background;
	float addtime = 0;
	int countElements;

	cWindow *topBar, *btmBar;
	cWindow* infoField;
	cDinamicLayer *layer;
	cocos2d::ClippingNode *scissors;

public:
	cButton *btnSettings, *btnBack;

	cGameScene();
	~cGameScene();
	cocos2d::Scene* bs_create();
	void update(float dt);
};


#endif // !GAME_SCENE_HPP