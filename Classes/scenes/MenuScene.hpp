#ifndef MENU_SCENE_HPP
#define MENU_SCENE_HPP

#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseScene.hpp"
#include "baseClasses/ui/UI.hpp"

#include "baseClasses/profile/Profile.hpp"
#include "baseClasses/calendar/Calendar.hpp"
#include "baseClasses/cards/BaseCards.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "baseClasses/inventory/Inventory.hpp"


class cMenuScene : public cBaseScene
{
private:
	cCreateProfileDialog * prof;
	cocos2d::Rect mdlWndRect;
	cWindow *bgWnd;

	cocos2d::Label *wndName;
	cocos2d::Sprite *background;
	cDropDown *dd;
	std::map<std::string,cBaseNode*> subMenu;
	cPlayDialog *playDialog;
public:
	cMenuScene();
	~cMenuScene();
	cocos2d::Scene* bs_create();
	void update(float dt);
};


#endif // !MENU_SCENE_HPP
