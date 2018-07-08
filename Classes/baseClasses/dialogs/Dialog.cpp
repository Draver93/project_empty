#include "Dialog.hpp"

cCreateProfileDialog::cCreateProfileDialog()
{
	complete = false;
	sex = false;
	choose = 0;
	sound = false, tutorial = false;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto center = cocos2d::Point(visibleSize.width / 2.0f + origin.x, visibleSize.height / 2.0f + origin.y);
    
    cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;

	this->setLocalZOrder(100);
	this->scheduleUpdate();

	float indent = visibleSize.height / 70.0f;
	float halfindent = indent / 2.0f;

	layer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 230));
	layer->setLocalZOrder(0);
	this->addChild(layer);

	cocos2d::Size bgSize = { visibleSize.width - indent * 2.0f, visibleSize.height / 4.0f };
	cWindow *bg = new cWindow(bgSize, W_MUTE, 10);
	bg->setCascadeOpacityEnabled(false);
	auto ch = bg->getChildren();
	for (auto it : ch) it->setOpacity(150);
	bg->setPosition(visibleSize / 2.0f);
	bg->setLocalZOrder(1);
	this->addChild(bg);
	

	cocos2d::Size wAvatarSize = { (bgSize.height - indent * 3.0f) * 0.8f, (bgSize.height - indent * 3.0f) * 0.8f };
	cocos2d::Size btnSize = cocos2d::Size((wAvatarSize.width - indent) / 2.0f, ((bgSize.height - indent * 3.0f) * 0.2f));

	float topBarH = visibleSize.height / 20.0f;
	cButton *btnExit;
	//btn exit
	{
		btnExit = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, -2);
		//listeners.push_back(btnExit->listener);
		cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnExit);
		btnExit->setPosition(-visibleSize.width / 2.0f + btnExitRect.size.width / 2.0f + indent, visibleSize.height /2.0f - btnExitRect.size.height / 2.0f - indent);
		btnExit->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
		bg->addChild(btnExit);
		btnExit->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{		
			return true;
		};

		cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/exit.png");
		setNodeSize(icon_exit, btnExit->wnd->getWSize().width * 0.7f, true, true);
		icon_exit->setLocalZOrder(4);
		btnExit->addChild(icon_exit);
	}

	cButton *btnSetting;
	//btn settings
	{
		btnSetting = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, -2);
		//listeners.push_back(btnSetting->listener);
		cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnSetting);
		btnSetting->setPosition(visibleSize.width / 2.0f - btnExitRect.size.width / 2.0f - indent, visibleSize.height / 2.0f - btnExitRect.size.height / 2.0f - indent);
		btnSetting->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		bg->addChild(btnSetting);

		cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/settings.png");
		setNodeSize(icon_exit, btnSetting->wnd->getWSize().width * 0.7f, true, true);
		icon_exit->setLocalZOrder(4);
		btnSetting->addChild(icon_exit);
	}

	cWindow *topBar;
	//top bar
	{
		cocos2d::Size tSize = cocos2d::Size(visibleSize.width - indent * 4.0f - topBarH * 2.0f, topBarH);
		topBar = new cWindow(tSize, W_MUTE, -2);
		cocos2d::Rect topBarRect = cocos2d::utils::getCascadeBoundingBox(topBar);
		topBar->setPosition(btnExit->getPosition().x + topBarH / 2.0f + indent + tSize.width / 2.0f, btnExit->getPosition().y);
		topBar->setOpacity(100);
		bg->addChild(topBar);

		auto gTtfConfig = new cocos2d::TTFConfig("fonts/chalk.ttf", 50);
		cocos2d::Label *wndName = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Создание профиля", cocos2d::TextHAlignment::CENTER);
		setNodeSize(wndName, topBar->getWSize().height * 0.5f, true, false);
		wndName->setPosition(topBar->getPosition());
		wndName->setLocalZOrder(3);

		bg->addChild(wndName);
	}

	cWindow *wndAvatar;
	{
		wndAvatar = new cWindow(wAvatarSize, W_MUTE, 10);
		wndAvatar->setPosition(-bgSize.width / 2.0f + indent + wAvatarSize.width /2.0f,
								bgSize.height / 2.0f - indent - wAvatarSize.height / 2.0f);
		wndAvatar->setLocalZOrder(2);
		wndAvatar->setCascadeOpacityEnabled(false);
		auto ch = wndAvatar->getChildren();
		for (auto it : ch) it->setOpacity(150);
		bg->addChild(wndAvatar);

		for (unsigned int i = 0; i < 4; i++)
		{
			std::string name = "avatars/" + std::to_string(i + 1) + ".png";
			cocos2d::Sprite *ava = cocos2d::Sprite::create(name);
			setNodeSize(ava, wAvatarSize.width, false, true);
			ava->setName(name);
			wndAvatar->addChild(ava);
			ava->setVisible(false);
			avatars.push_back(ava);
		}
		avatars.at(choose)->setVisible(true);

		cButton *left = new cButton(nullptr, btnSize, 1.05f, -2);
		//this->listeners.push_back(left->listener);
		left->setPosition(-bgSize.width / 2.0f + indent + btnSize.width / 2.0f,
						  -bgSize.height / 2.0f + indent + btnSize.height / 2.0f);
		bg->addChild(left);
		left->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		cocos2d::Label* prev = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Пред.", cocos2d::TextHAlignment::CENTER);
		setNodeSize(prev, btnSize.width * 0.9f, true, true);
		left->setLocalZOrder(2);
		left->addChild(prev);

		cButton *right = new cButton(nullptr, btnSize, 1.05f, -2);
		//this->listeners.push_back(right->listener);
		right->setPosition(left->getPosition().x + indent + btnSize.width, left->getPosition().y);
		bg->addChild(right);
		right->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		cocos2d::Label* next = cocos2d::Label::createWithTTF(*gTtfConfig, u8"След.", cocos2d::TextHAlignment::CENTER);
		setNodeSize(next, btnSize.width * 0.9f, true, true);
		right->setLocalZOrder(2);
		right->addChild(next);

		left->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			if (choose - 1 >= 0)
			{
				for (auto it : avatars) it->setVisible(false);
				choose--;
				avatars.at(choose)->setVisible(true);
			}

			if (choose < 1) left->wnd->setWColor(cocos2d::Color3B(150, 150, 150));
			else left->wnd->setWColor(cocos2d::Color3B(96, 139, 171));

			if (choose > (int)avatars.size() - 2) right->wnd->setWColor(cocos2d::Color3B(150, 150, 150));
			else right->wnd->setWColor(cocos2d::Color3B(96, 139, 171));

			return true;
		};
		right->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			if (choose + 1 < (int)avatars.size())
			{
				for (auto it : avatars) it->setVisible(false);
				choose++;
				avatars.at(choose)->setVisible(true);
				right->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
			}

			if (choose < 1) left->wnd->setWColor(cocos2d::Color3B(150, 150, 150));
			else left->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
			
			if (choose > (int)avatars.size() - 2) right->wnd->setWColor(cocos2d::Color3B(150, 150, 150));
			else right->wnd->setWColor(cocos2d::Color3B(96, 139, 171));

			return true;
		};
	}

	{
		cocos2d::Size wndDataSize = { bgSize.width - indent * 3.0f - wAvatarSize.width, (bgSize.height - indent * 2.0f) };
		cWindow *wndData = new cWindow(wndDataSize, W_MUTE, 10);
		wndData->setPosition(wndAvatar->getPosition().x + wAvatarSize.width / 2.0f + indent + wndDataSize.width / 2.0f, wndAvatar->getPosition().y - btnSize.height + indent);
		wndData->setLocalZOrder(2);
		wndData->setCascadeOpacityEnabled(false);
		auto ch = wndData->getChildren();
		for (auto it : ch) it->setOpacity(150);
		bg->addChild(wndData);

		int countEl = 4;
		cocos2d::Size elSize = { wndDataSize.width - indent * 2.0f, (wndDataSize.height - indent * (countEl + 1)) / countEl};
		std::vector<cWindow*> el;
		std::vector<std::string> names = { u8"Имя",u8"Пол",u8"  Звук  ",u8"Обучение" };
		for (int i = 0; i < countEl; i++)
		{			
			cocos2d::Size nameSize = { (elSize.width - indent) / 2.0f, elSize.height };
			cocos2d::Size valSize = { (elSize.width - indent) / 2.0f, elSize.height };
			cButton *name = new cButton(nullptr, nameSize, 1.05f, -2);
			name->setPosition(-wndDataSize.width / 2.0f + indent + nameSize.width / 2.0f, 
				wndDataSize.height / 2.0f - indent - nameSize.height / 2.0f - (elSize.height + indent) * i);
			name->wnd->setWColor(cocos2d::Color3B(96, 139, 171));

			cocos2d::Label* nameLabel = cocos2d::Label::createWithTTF(*gTtfConfig, names.at(i), cocos2d::TextHAlignment::CENTER);
			cocos2d::Size sz = nameLabel->getContentSize();
			if(sz.width - nameSize.width > 0) setNodeSize(nameLabel, nameSize.width * 0.9f, true, true);
			else setNodeSize(nameLabel, nameSize.height * 0.6f, true, false);
			name->addChild(nameLabel);


			cWindow *val = new cWindow(cocos2d::Size(valSize.width * 0.8f, valSize.height), W_MUTE, 10);
			val->setPosition(name->getPosition().x + nameSize.width /2.0f + valSize.width /2.0f + indent, name->getPosition().y);
			val->setOpacity(255);


			wndData->addChild(name);
			wndData->addChild(val);
			if (i == 0)
			{
				textField = cocos2d::ui::TextField::create(u8"Введите имя", "fonts/chalk.ttf", 30);
				textField->setLocalZOrder(10);
				textField->setPlaceHolderColor(cocos2d::Color3B(200, 200, 200));
				textField->setColor(cocos2d::Color3B(255, 255, 255));
				textField->attachWithIME();
				setNodeSize(textField, valSize.width * 0.7f, true, true);
				val->addChild(textField);
				textBox = val;

				name->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
				{
					textField->attachWithIME();
					return true;
				};
			}
			if (i == 1)
			{
				cocos2d::Size btnSz = cocos2d::Size(valSize.height, valSize.height);

				btnW = new cButton(nullptr, btnSz, 1.05f, -2);
				//this->listeners.push_back(btnW->listener);
				btnW->setPosition(val->getPosition().x + valSize.width / 4.0f, val->getPosition().y);
				wndData->addChild(btnW);
				btnW->wnd->setWColor(cocos2d::Color3B(70, 200, 70));
				cocos2d::Label* WLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Ж", cocos2d::TextHAlignment::CENTER);
				setNodeSize(WLabel, btnSize.height * 0.4f, true, false);
				btnW->addChild(WLabel);
				btnW->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
				{
					sex = false;
					btnW->wnd->setWColor(cocos2d::Color3B(70, 200, 70));
					btnM->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
					return true;
				};

				btnM = new cButton(nullptr, btnSz, 1.05f, -2);
				//this->listeners.push_back(btnM->listener);
				btnM->setPosition(val->getPosition().x -valSize.width / 4.0f, val->getPosition().y);
				wndData->addChild(btnM);
				btnM->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
				cocos2d::Label* MLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"М", cocos2d::TextHAlignment::CENTER);
				setNodeSize(MLabel, btnSize.height * 0.4f, true, false);
				btnM->addChild(MLabel);
				btnM->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
				{
					sex = true;
					btnM->wnd->setWColor(cocos2d::Color3B(70, 200, 70));
					btnW->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
					return true;
				};

				val->setVisible(false);
			}
			if (i == 2)
			{
				cocos2d::Size btnSz = cocos2d::Size(valSize.height, valSize.height);

				sound = new cCheckBox(cocos2d::Size(valSize.width * 0.8f, valSize.height), -2);
				//this->listeners.push_back(sound->listener);

				sound->setPosition(val->getPosition().x, val->getPosition().y);
				sound->setActive(false);
				sound->setFinalPos();
				wndData->addChild(sound);

				val->setVisible(false);
			}
			if (i == 3)
			{
				cocos2d::Size btnSz = cocos2d::Size(valSize.height, valSize.height);

				tutorial = new cCheckBox(cocos2d::Size(valSize.width * 0.8f, valSize.height), -2);
				//this->listeners.push_back(tutorial->listener);

				tutorial->setPosition(val->getPosition().x, val->getPosition().y);
				tutorial->setActive(false);
				tutorial->setFinalPos();
				wndData->addChild(tutorial);

				val->setVisible(false);
			}
		}
	}


	cocos2d::Size acceptBtnSize = { bgSize.width / 4.0f, bgSize.height / 5.0f};
	cButton * acceptBtn = new cButton(nullptr, acceptBtnSize, 1.05f, -2);
	acceptBtn->setPosition(bgSize.width / 2.0f - acceptBtnSize.width / 2.0f - indent, -bgSize.height / 2.0f - acceptBtnSize.height / 2.0 - indent);
	bg->addChild(acceptBtn);
	acceptBtn->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
	cocos2d::Label* label_accept = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Сохранить.", cocos2d::TextHAlignment::CENTER);
	setNodeSize(label_accept, acceptBtnSize.width * 0.9f, true, true);
	acceptBtn->setLocalZOrder(2);
	acceptBtn->addChild(label_accept);
	acceptBtn->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
	{
		if(textField->getString() == "")return true;
		userDefault->profile["sName"] = textField->getString();
		userDefault->profile["bSex"] = sex;
		userDefault->profile["sAvatarName"] = avatars.at(choose)->getName();
		userDefault->profile["bSound"] = sound->getActive();
		userDefault->profile["bTutorial"] = tutorial->getActive();
		userDefault->saveProfile();
		this->hide(0);
		complete = true;
		return true;
	};


	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		return true;
	};

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -1);
	show(0);

	secondUpdate = [=](float dt)
	{
		cocos2d::Size sz = textField->getContentSize();
		std::string str = textField->getString();
		if (str.empty()) textBox->setWColor(cocos2d::Color3B(247, 116, 98));
		else textBox->setWColor(cocos2d::Color3B(70, 200, 70));

		cocos2d::Size nameSize = textBox->getWSize();
		float locIndent = nameSize.height / 2.0f;

		setNodeSize(textField, nameSize.width - locIndent, true, true);
		sz = textField->getContentSize() * textField->getScale();
		if (sz.height - nameSize.height + locIndent > 0)
			setNodeSize(textField, nameSize.height - locIndent, true, false);
		return;
	};

}
cCreateProfileDialog::~cCreateProfileDialog()
{
}
void cCreateProfileDialog::show(float time)
{
	//for (auto it : listeners)it->setEnabled(true);
	this->setVisible(true);
	//listener->setEnabled(true);
	textField->setEnabled(true);
}
void cCreateProfileDialog::hide(float time)
{
	this->setVisible(false);
	//Start->listener->setEnabled(false);
	//listener->setEnabled(false);

	//for (auto it : listeners)it->setEnabled(false);
	textField->setEnabled(false);

}

/////

cPlayDialog::cPlayDialog()
{
	isHide = true;
	this->setLocalZOrder(100);

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto center = cocos2d::Point(visibleSize.width / 2.0f + origin.x, visibleSize.height / 2.0f + origin.y);
    
    cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;
    
	float indent = visibleSize.height / 70.0f;
	float halfindent = indent / 2.0f;

	layer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 230));
	layer->setLocalZOrder(0);
	layer->setPosition(-(visibleSize / 2.0f).width, -(visibleSize / 2.0f).height);
	this->addChild(layer);
	this->setPosition(visibleSize / 2.0f);

	float topBarH = visibleSize.height / 20.0f;
	cButton *btnExit;
	//btn exit
	{
		btnExit = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, -2);
		//listeners.push_back(btnExit->listener);
		cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnExit);
		btnExit->setPosition(-visibleSize.width / 2.0f + btnExitRect.size.width / 2.0f + indent, visibleSize.height / 2.0f - btnExitRect.size.height / 2.0f - indent);
		btnExit->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
		this->addChild(btnExit);
		btnExit->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			hide(0);
			cUserDefault *userDefault = cUserDefault::getInstance();
			userDefault->activeMode = nullptr;
			return true;
		};

		cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/back.png");
		setNodeSize(icon_exit, btnExit->wnd->getWSize().width * 0.7f, true, true);
		icon_exit->setLocalZOrder(4);
		btnExit->addChild(icon_exit);
	}

	cButton *btnSetting;
	//btn settings
	{
		btnSetting = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, -2);
		//listeners.push_back(btnSetting->listener);
		cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnSetting);
		btnSetting->setPosition(visibleSize.width / 2.0f - btnExitRect.size.width / 2.0f - indent, visibleSize.height / 2.0f - btnExitRect.size.height / 2.0f - indent);
		btnSetting->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		this->addChild(btnSetting);

		cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/settings.png");
		setNodeSize(icon_exit, btnSetting->wnd->getWSize().width * 0.7f, true, true);
		icon_exit->setLocalZOrder(4);
		btnSetting->addChild(icon_exit);
	}

	cWindow *topBar;
	//top bar
	{
		cocos2d::Size tSize = cocos2d::Size(visibleSize.width - indent * 4.0f - topBarH * 2.0f, topBarH);
		topBar = new cWindow(tSize, W_MUTE, -2);
		cocos2d::Rect topBarRect = cocos2d::utils::getCascadeBoundingBox(topBar);
		topBar->setPosition(btnExit->getPosition().x + topBarH / 2.0f + indent + tSize.width / 2.0f, btnExit->getPosition().y);
		topBar->setCascadeOpacityEnabled(false);
		auto ch = topBar->getChildren();
		for (auto it : ch) it->setOpacity(100);
		this->addChild(topBar);

        cUserDefault *userDefault = cUserDefault::getInstance();
	    auto gTtfConfig = userDefault->font;
        
		cocos2d::Label *wndName = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Выбор игры: День 15 из 45", cocos2d::TextHAlignment::CENTER);
		setNodeSize(wndName, topBar->getWSize().height * 0.5f, true, false);
		//wndName->setLocalZOrder(3);

		topBar->addChild(wndName);
	}

	userDefault->updateCurrentModes();
	float countNodes = 3;
	if (userDefault->modes.size() > 3)countNodes = userDefault->modes.size();


	float scale = scale = countNodes / 3.0f;
 	 
	cocos2d::Size btnSpace = { visibleSize.width * 0.9f, visibleSize.height - topBarH * 2.0f };
	int x = 1, y = 0;
	{
		int count = userDefault->modes.size();
		float w = btnSpace.width, h = btnSpace.height;

		
		for (x = 1; x < count + 1; x++)
		{
			y = h / (w / x);
			if (y * x >= count) break;
		}
	}

	cBaseNode *node = new cBaseNode();
	node->setPosition(0, -topBarH);
	this->addChild(node);

	int loc_x = 0, loc_y = 0;
	cocos2d::Size btnSize = { btnSpace.width / (x + 1),  btnSpace.width / (x + 1) };
	for (auto it : userDefault->modes)
	{
		if (loc_x >= x) { loc_x = 0; loc_y += 1; }

		cDinamicLayer* layer = it->getModeIntro(btnSize);
		node->addChild(layer);
		modeEl* el = new modeEl();
		el->layer = layer;
		el->originSize = btnSize;
		el->originPos = cocos2d::Vec2(-btnSpace.width / 2.0f + (btnSpace.width / x / 2.0f) + (btnSpace.width / x * loc_x),
										btnSpace.height / 2.0f - (btnSpace.width / x / 2.0f) - (btnSpace.width / x * loc_y));
		
		modesVec.push_back(el);

		layer->setPosition(el->originPos);

		loc_x++;
	}


	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) { return true; };
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -1);

	secondUpdate = [=](float dt)
	{
		cUserDefault *userDefault = cUserDefault::getInstance();

		for (auto it : modesVec)
		{
			it->update(dt, it->originSize.width * 0.3f);
			it->layer->step(dt);
		}
	};

	hide(0);
}
cPlayDialog::~cPlayDialog()
{
}
void cPlayDialog::show(float time)
{
	cUserDefault *userDefault = cUserDefault::getInstance();
	userDefault->activeMode = nullptr;
	isHide = false;
	//for (auto it : listeners)it->setEnabled(true);
	this->setVisible(true);
	//listener->setEnabled(true);
}
void cPlayDialog::hide(float time)
{
	isHide = true;
	//for (auto it : listeners)it->setEnabled(false);
	this->setVisible(false);
	//listener->setEnabled(false);
}

/////

cEndDialog::cEndDialog()
{
	outro = nullptr;
	createElements();

	cUserDefault *userDefault = cUserDefault::getInstance();
	outro = userDefault->activeMode->getModeOutro(cocos2d::Size(0, 0));
	this->addChild(outro);
	//Level
	/*//commented +
	{
	cocos2d::Size bgLvlSize = { bgSize.width - 2.0f * indent, (mdlInterface - indent * 4) * 0.25f };
	cWindow* levelBg = new cWindow(bgLvlSize, W_MUTE, 10);
	levelBg->setPosition(0, bgSize.height / 2.0f - indent - bgLvlSize.height / 2.0f);
	ch = levelBg->getChildren();
	for (auto it : ch) it->setOpacity(80);
	bg->addChild(levelBg);
	cUserDefault *userDefault = cUserDefault::getInstance();
	userDefault->exam.gameLevel;

	cocos2d::Label* levelLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Уровень: " + std::to_string(userDefault->exam.gameLevel.at(0)) + "-"
	+ std::to_string(userDefault->exam.gameLevel.at(1)) + "-"
	+ std::to_string(userDefault->exam.gameLevel.at(2)) + "-"
	+ std::to_string(userDefault->exam.gameLevel.at(3)), cocos2d::TextHAlignment::CENTER);
	setNodeInRect(levelLabel, bgLvlSize, bgLvlSize.height * 0.2f);
	levelBg->addChild(levelLabel);
	}

	//Score
	{
	cocos2d::Size bgScoreSize = { bgSize.width - 2.0f * indent, (mdlInterface - indent * 4) * 0.5f };
	cWindow* scoreBg = new cWindow(bgScoreSize, W_MUTE, 10);
	scoreBg->setPosition(0, 0);
	ch = scoreBg->getChildren();
	for (auto it : ch) it->setOpacity(80);
	bg->addChild(scoreBg);
	mode->currentScore;

	cocos2d::Label* scoreVal = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Счет:\n\n999999", cocos2d::TextHAlignment::LEFT);
	setNodeInRect(scoreVal, bgScoreSize, bgScoreSize.height * 0.4f);
	scoreBg->addChild(scoreVal);
	}

	//Best Subject
	{
	cocos2d::Size bgSubSize = { bgSize.width - 2.0f * indent, (mdlInterface - indent * 4) * 0.25f };
	cWindow* subjectBg = new cWindow(bgSubSize, W_MUTE, 10);
	subjectBg->setPosition(0, -bgSize.height / 2.0f + indent + bgSubSize.height / 2.0f);
	ch = subjectBg->getChildren();
	for (auto it : ch) it->setOpacity(80);
	bg->addChild(subjectBg);
	//mode->cards<>;
	}
	//commented -*/

	secondUpdate = [=](float dt)
	{
		if (outro != nullptr)outro->step(dt);
	};
}
cEndDialog::~cEndDialog()
{
}
void cEndDialog::show(float time)
{
	//for (auto it : listeners)it->setEnabled(true);
	this->setVisible(true);
}
void cEndDialog::hide(float time)
{
	//for (auto it : listeners)it->setEnabled(false);
	this->setVisible(false);
}
void cEndDialog::createElements()
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto center = cocos2d::Point(visibleSize.width / 2.0f + origin.x, visibleSize.height / 2.0f + origin.y);
    
    cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;
    
	this->setLocalZOrder(100);
	this->scheduleUpdate();

	float indent = visibleSize.height / 70.0f;
	float halfindent = indent / 2.0f;

	layer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 230));
	layer->setLocalZOrder(-5);
	layer->setPosition(-(visibleSize / 2.0f).width, -(visibleSize / 2.0f).height);
	this->addChild(layer);
	this->setPosition(visibleSize / 2.0f);

	/*
	float topBarH = visibleSize.height / 20.0f;
	//top line
	{
		cButton *btnExit;
		//btn exit
		{
			btnExit = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, -2);
			//listeners.push_back(btnExit->listener);
			cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnExit);
			btnExit->setPosition(-visibleSize.width / 2.0f + btnExitRect.size.width / 2.0f + indent, visibleSize.height / 2.0f - btnExitRect.size.height / 2.0f - indent);
			btnExit->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
			this->addChild(btnExit);
			btnExit->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
			{
				cSceneManager *sceneManager = cSceneManager::getInstance();
				sceneManager->changeScene("menu");
				hide(0);
				return true;
			};

			cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/exit.png");
			setNodeSize(icon_exit, btnExit->wnd->getWSize().width * 0.7f, true, true);
			icon_exit->setLocalZOrder(4);
			btnExit->addChild(icon_exit);
		}

		cButton *btnSetting;
		//btn settings
		{
			btnSetting = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, -2);
			//listeners.push_back(btnSetting->listener);
			cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnSetting);
			btnSetting->setPosition(visibleSize.width / 2.0f - btnExitRect.size.width / 2.0f - indent, visibleSize.height / 2.0f - btnExitRect.size.height / 2.0f - indent);
			btnSetting->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
			this->addChild(btnSetting);

			cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/settings.png");
			setNodeSize(icon_exit, btnSetting->wnd->getWSize().width * 0.7f, true, true);
			icon_exit->setLocalZOrder(4);
			btnSetting->addChild(icon_exit);
		}

		cWindow *topBar;
		//top bar
		{
			cocos2d::Size tSize = cocos2d::Size(visibleSize.width - indent * 4.0f - topBarH * 2.0f, topBarH);
			topBar = new cWindow(tSize, W_MUTE, -2);
			cocos2d::Rect topBarRect = cocos2d::utils::getCascadeBoundingBox(topBar);
			topBar->setPosition(btnExit->getPosition().x + topBarH / 2.0f + indent + tSize.width / 2.0f, btnExit->getPosition().y);
			topBar->setOpacity(100);
			this->addChild(topBar);

            cUserDefault *userDefault = cUserDefault::getInstance();
	        auto gTtfConfig = userDefault->font;
            
			cocos2d::Label *wndName = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Результат", cocos2d::TextHAlignment::CENTER);
			setNodeSize(wndName, topBar->getWSize().height * 0.5f, true, false);
			wndName->setPosition(topBar->getPosition());
			wndName->setLocalZOrder(3);

			this->addChild(wndName);
		}
	}
	*/



	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		return true;
	};
	listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -1);
}


