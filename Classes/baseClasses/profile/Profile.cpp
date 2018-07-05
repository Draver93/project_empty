#include "Profile.hpp"

cProfile::cProfile(cocos2d::Size sizeProfile)
{
	activeZone = -1;
	size = sizeProfile;
	statElementShowId = 0;
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;
	auto gTtfConfigHd = userDefault->fontHd;
	std::string avatarFilePath = userDefault->profile["sAvatarName"];
	std::string nameData = userDefault->profile["sName"];

	//avatar
	cocos2d::Sprite *sAvatar = cocos2d::Sprite::create(avatarFilePath);
	if (sAvatar == nullptr)
	{
		firstStart = true;
		return;
	}

	{
		sAvatar->setCascadeOpacityEnabled(false);
		sAvatar->setName("ava");
		setNodeInRect(sAvatar, sizeProfile * 0.6f, 0);
		sAvatar->setPosition(0, sizeProfile.height * 0.05f);
		sAvatar->setLocalZOrder(2);
		this->addChild(sAvatar);

		cocos2d::Size avaSize = sAvatar->getContentSize() *  sAvatar->getScale();

		cocos2d::Sprite *sFloor = cocos2d::Sprite::create("icons/floor.png");
		sFloor->setContentSize(cocos2d::Size(avaSize.width * 2.5f, avaSize.width * 0.8f));
		sFloor->setPosition(0, sAvatar->getPosition().y - sizeProfile.height * 0.30f);
		sFloor->setLocalZOrder(0);
		this->addChild(sFloor);

		stats.size = cocos2d::Size(sFloor->getContentSize().width * sFloor->getScaleX(), sFloor->getContentSize().height * sFloor->getScaleY());
		stats.pos = sFloor->getPosition();

		

		infoElevent *el = new infoElevent();
		el->node = new cBaseNode();

		cocos2d::Label *profileName = cocos2d::Label::createWithTTF(*gTtfConfig, nameData, cocos2d::TextHAlignment::CENTER);
		setNodeInRect(profileName, sizeProfile * 0.25f, 0);
		el->node->addChild(profileName);

		el->node->setPosition(sizeProfile.width / 4.0f, sAvatar->getPosition().y + avaSize.height / 2.0f);
		el->originPos = el->node->getPosition();
		this->addChild(el->node);
		infoElements.push_back(el);
	}

	//Stats
	{
		std::vector<std::string> arrayNames = { u8"Естествозн.", u8"Информ.", u8"Искусство", u8"Математ.", u8"Обществозн.", u8"Труд", u8"Физ-ра", u8"Филолог." };
		// Отображение 8 статов
		for (int i = 0; i < 8; i++)
		{
			cocos2d::Sprite *sElem = cocos2d::Sprite::create("gameScreen/" + std::to_string(i + 1) + ".png");
			sElem->setName("el" + std::to_string(i + 1));
			//setNodeInRect(sElem, cocos2d::Size(avaSize.width / 7.0f, avaSize.width / 7.0f), 0);
			this->addChild(sElem);
		}
	}

	//Info
	{
		cocos2d::Size avaSize = sAvatar->getContentSize() *  sAvatar->getScale();
		float average = userDefault->profile["fAverageScore"];
		float best = userDefault->profile["fBestScore"];
		float last = userDefault->profile["fLastScore"];
		{
			infoElevent *el = new infoElevent();
			el->node = new cBaseNode();

			cocos2d::Label *aScore = cocos2d::Label::createWithTTF(*gTtfConfigHd, std::to_string(average).substr(0, 3), cocos2d::TextHAlignment::CENTER);
			setNodeInRect(aScore, sizeProfile * 0.3f, 0);
			aScore->setLocalZOrder(2);
			el->node->addChild(aScore);
			auto sz = aScore->getContentSize() * aScore->getScale();

			cocos2d::Label *aScoreL = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Сред. б.", cocos2d::TextHAlignment::CENTER);
			setNodeInRect(aScoreL, sz / 1.5f, 0);
			auto szL = aScoreL->getContentSize() * aScoreL->getScale();
			aScoreL->setPosition(-sz.width / 2.0f + szL.width / 2.0f, sz.height / 2.0f + szL.height * 0.8f);
			el->node->addChild(aScoreL);


			el->node->setPosition(sAvatar->getPosition().x + sizeProfile.width * 0.3f, sAvatar->getPosition().y);
			el->originPos = el->node->getPosition();
			this->addChild(el->node);
			infoElements.push_back(el);
		}

		{
			infoElevent *el = new infoElevent();
			el->node = new cBaseNode();

			cocos2d::Label *bScore = cocos2d::Label::createWithTTF(*gTtfConfigHd, std::to_string(best).substr(0, 6), cocos2d::TextHAlignment::CENTER);
			setNodeInRect(bScore, sizeProfile * 0.3f, 0);
			bScore->setLocalZOrder(2);
			el->node->addChild(bScore);
			auto sz = bScore->getContentSize() * bScore->getScale();

			cocos2d::Label *bScoreL = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Лучш. сч.", cocos2d::TextHAlignment::CENTER);
			setNodeInRect(bScoreL, sz / 1.5f, 0);
			auto szL = bScoreL->getContentSize() * bScoreL->getScale();
			bScoreL->setPosition(-sz.width / 2.0f + szL.width / 2.0f, sz.height / 2.0f + szL.height * 0.8f);
			el->node->addChild(bScoreL);

			el->node->setPosition(sAvatar->getPosition().x - sizeProfile.width * 0.3f, sAvatar->getPosition().y + avaSize.height / 4.0f);
			el->originPos = el->node->getPosition();
			this->addChild(el->node);
			infoElements.push_back(el);
		}

		{
			infoElevent *el = new infoElevent();
			el->node = new cBaseNode();

			cocos2d::Label *lScore = cocos2d::Label::createWithTTF(*gTtfConfigHd, std::to_string(last).substr(0, 6), cocos2d::TextHAlignment::CENTER);
			setNodeInRect(lScore, sizeProfile * 0.3f, 0);
			lScore->setLocalZOrder(2);
			el->node->addChild(lScore);
			auto sz = lScore->getContentSize() * lScore->getScale();

			cocos2d::Label *lScoreL = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Посл. сч.", cocos2d::TextHAlignment::CENTER);
			setNodeInRect(lScoreL, sz / 1.5f, 0);
			auto szL = lScoreL->getContentSize() * lScoreL->getScale();
			lScoreL->setPosition(-sz.width / 2.0f + szL.width / 2.0f,sz.height / 2.0f + szL.height * 0.8f);
			el->node->addChild(lScoreL);

			el->node->setPosition(sAvatar->getPosition().x - sizeProfile.width * 0.3f, sAvatar->getPosition().y - avaSize.height / 4.0f);
			el->originPos = el->node->getPosition();
			this->addChild(el->node);
			infoElements.push_back(el);
		}

		{
			float indent = visibleSize.height / 70.0f;

			float h = avaSize.width / 4.0f;
			statValLabel = cocos2d::Label::createWithTTF(*gTtfConfig, "0.0", cocos2d::TextHAlignment::CENTER);
			setNodeSize(statValLabel, h, true, false);
			statValLabel->setPosition(0, -size.height / 2.0f + indent + h /2.0f);
			this->addChild(statValLabel);
		}
	}

	//Diag
	{
		auto stats = userDefault->profile["aStats"];
		cocos2d::Size avaSize = sAvatar->getContentSize() *  sAvatar->getScale();

		for (int i = 0; i < 8; i++)profileSkills.push_back(stats.at(i));
		float wdt = sizeProfile.width < sizeProfile.height / 1.5f ? sizeProfile.width : sizeProfile.height / 1.5f;
		diagram = new cDiagram(&profileSkills, wdt * 0.8f);
		diagram->setOpacity(150);
		diagram->setPosition(sAvatar->getPosition().x, sAvatar->getPosition().y);
		diagram->setLocalZOrder(0);
		diagram->setLocalZOrder(-1);
		this->addChild(diagram);
	}

	this->listener = cocos2d::EventListenerTouchOneByOne::create();
	this->listener->setSwallowTouches(true);
	this->listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		p = this->convertToNodeSpace(p);
		
		cocos2d::Rect rectStats;
		rectStats.size = cocos2d::Size(size.width, size.height * 0.3f);
		rectStats.origin = cocos2d::Vec2(-size.width / 2.0f, -size.height / 2.0f);

		cocos2d::Rect rectAvatar;
		rectAvatar.size = cocos2d::Size(size.width, size.height * 0.7f); //orig left down size up right
		rectAvatar.origin = cocos2d::Vec2(-size.width / 2.0f, size.height / 2.0f - rectAvatar.size.height);

		if (rectStats.containsPoint(p))
		{
			stats.stAngle = stats.statAngle;
			startPoint = p;
			activeZone = 0;
			return true;
		}
		if (rectAvatar.containsPoint(p))
		{
			startPoint = p;
			auto node = this->getChildByName("ava");
			auto ang = node->getRotation3D();
			avatarData.stAngle = ang.y;
			activeZone = 1;
			return true;
		}
		return false;
	};
	this->listener->onTouchMoved = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		p = this->convertToNodeSpace(p);

		if (activeZone == 0) //stats
		{
			auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
			float step = visibleSize.width / 180.0f;
			auto dS = p - startPoint;
			stats.dtAngle = dS.x / step;
			stats.statAngle = stats.stAngle - stats.dtAngle;

			if (stats.dtAngle > 0)stats.direct = false;
			else if (stats.dtAngle < 0)stats.direct = true;

			if (stats.statAngle > 360) 
				stats.statAngle -= 360.0f * (int)(stats.statAngle / 360.0f);
			else if (stats.statAngle < 0) 
				stats.statAngle = 360.0f + (360.0f * (int)(stats.statAngle / -360.0f) + stats.statAngle);
		}
		else if (activeZone == 1)
		{
			float maxAngle = 30;

			auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
			auto node = this->getChildByName("ava");
			auto ang = node->getRotation3D();
			
			float dtAngle;
			auto dS = p - startPoint;
			float step = visibleSize.width / maxAngle;
			float addAngle = dS.x / step;

			if(avatarData.stAngle + addAngle > maxAngle)node->setRotation3D(cocos2d::Vec3(0, maxAngle, 0));
			else if(avatarData.stAngle + addAngle < -maxAngle)node->setRotation3D(cocos2d::Vec3(0, -maxAngle, 0));
			else node->setRotation3D(cocos2d::Vec3(0, avatarData.stAngle + addAngle, 0));
			
		}
		return true;
	};
	this->listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		endPoint = this->convertToNodeSpace(p);

		activeZone = -1;
		return true;
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->listener, 1);

	secondUpdate = [&](float dt)
	{
		statVal.update(dt);

		if (activeZone != 0)
		{
			if(stats.direct) stats.statAngle += 10.0f * dt;
			else stats.statAngle -= 10.0f * dt;
		}
		if (activeZone != 1)
		{
			float speed = 50.0f;
			auto node = this->getChildByName("ava");
			auto ang = node->getRotation3D();
	
			if(speed * dt > abs(ang.y)) node->setRotation3D(cocos2d::Vec3(0, 0, 0));
			else
			{
				if (ang.y > 0)node->setRotation3D(cocos2d::Vec3(0, ang.y - speed * dt, 0));
				else if (ang.y < 0)node->setRotation3D(cocos2d::Vec3(0, ang.y + speed * dt, 0));
			}
		}
		auto node = this->getChildByName("ava");
		auto avaSize = node->getContentSize() * node->getScale();
		float indent = 360.0f / 8.0f;
		if (stats.statAngle > 360)stats.statAngle = 0;
		else if(stats.statAngle < 0)stats.statAngle = 360;

		int centralElement = -1;
		int centralElementAngle = -1;
		for (int i = 0; i < 8; i++)
		{
			float angle = stats.statAngle + i * indent;
			int fullCicle = angle / 360;
			angle -= 360 * fullCicle;
			if (angle < 180 + 10.0f && angle > 180 - 10.0f)
			{
				centralElementAngle = angle;
				centralElement = i;
			}
			float rad = angle * (b2_pi / 180.0f);
			float x = sin(rad), y = cos(rad); // -1 1

			cocos2d::Node *node = nullptr;
			node = this->getChildByName("el" + std::to_string(i + 1));
			if (node == nullptr) continue;

			float wdth = avaSize.width / (1.5f + 1.5f * ((y + 1.0f) / 2.0f));
			setNodeSize(node, wdth, true, true);

			if (y > 0.0f) node->setLocalZOrder(-1);
			else node->setLocalZOrder(3);

			node->setPosition(stats.pos.x + (x * (stats.size.width + wdth) / 2.0f), stats.pos.y + (y * stats.size.height / 2.0f));
		}

		for(auto it : infoElements)
		{
			it->update(dt, indent / 4.0f);
		}

		//float angle = stats.statAngle + i * indent;
		if (statElementShowId != centralElement)
		{
			if(centralElement == -1) statVal.set(0, 0.5f);
			else statVal.set(profileSkills.at(centralElement), 0.5f);
			statElementShowId = centralElement;
		}
		//if(statVal.isProgress())
		{
			statValLabel->setString(std::to_string(statVal.get()).substr(0, std::to_string(statVal.get()).find('.') + 2));
			if (centralElement != -1)
			{
				float proc = 100.0f - (abs(180.0f - centralElementAngle) / (10.0f / 100.0f));
				int op = 255.0f / 50.0f * proc;
				if (op > 255)op = 255; if (op < 0)op = 0;

				statValLabel->setOpacity(op);
				cocos2d::Node *node = this->getChildByName("el" + std::to_string(centralElement + 1));
				statValLabel->setPosition(node->getPosition().x, statValLabel->getPosition().y);
			}
			else statValLabel->setOpacity(0);
		}
	};
}

cProfile::~cProfile()
{
}

