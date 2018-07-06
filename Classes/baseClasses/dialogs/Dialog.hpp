#ifndef DIALOG_HPP
#define DIALOG_HPP

#include "cocos2d.h"
#include "string"
#include "vector"
#include "map"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/UserDefault.hpp"
#include "SceneManager.hpp"
#include <ui/UITextField.h>
#include "baseClasses/digitEx/cDigit.hpp"

class cCreateProfileDialog : public cBaseNode
{
private:
	cocos2d::LayerColor *layer;

	std::vector<cocos2d::Sprite*> avatars;
	int choose;

	cocos2d::ui::TextField *textField;
	cWindow *textBox;

	cButton *Start;
	bool sex;
	cButton *btnM, *btnW;
	cCheckBox *tutorial, *sound;
public:
	bool complete;
	cCreateProfileDialog();
	~cCreateProfileDialog();

	void show(float time);
	void hide(float time);

};

class cPlayDialog : public cBaseNode
{
private:
	bool isHide;
	cBaseScene *pScene;
	cocos2d::LayerColor *layer;

	struct modeEl 
	{
		cocos2d::Size originSize;
		cocos2d::Vec2 originPos;
		cDigit x, y;
		cDinamicLayer *layer;
		~modeEl()
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

				x.set(ind, (rand() % 4) + 3);
			}
			if (!y.isProgress())
			{
				int proc = rand() % 100;
				float ind = (indent / 100.0f) * proc - indent / 2.0f;

				y.set(ind, (rand() % 4) + 3);
			}
			layer->setPosition(originPos.x + x.get(), originPos.y + y.get());
		};
	};
	std::vector<modeEl*> modesVec;

public:
	cPlayDialog();
	~cPlayDialog();

	void show(float time);
	void hide(float time);

};

class cEndDialog : public cBaseNode
{
private:
	cocos2d::LayerColor *layer;
	cButton *exitBtn, *restartBtn;
	cDinamicLayer *outro;

	void createElements();
public:
	cEndDialog();
	~cEndDialog();

	void show(float time);
	void hide(float time);

};

class cExitLevelDialog : public cBaseNode
{
private:
	cocos2d::LayerColor *layer;

public:
};

class cExitGameDialog : public cBaseNode
{
private:
	cocos2d::LayerColor *layer;

public:
};

#endif // !DIALOG_HPP
