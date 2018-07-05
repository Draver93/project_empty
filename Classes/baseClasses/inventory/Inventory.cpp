#include "Inventory.hpp"

cCardInventory::cCardInventory(cocos2d::Size invSize)
{
	size = invSize;
	isPress = false;
	lastListPos[0] = lastListPos[1] = 0;
	centerCard = centerSC = 0;
	for (int i = 0; i < 5; i++)time_points[i] = 0;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;

	load();
	
	//Ratio
	cocos2d::Sprite *back = cocos2d::Sprite::create("cards/btm.png");
	float hightRatio = back->getContentSize().height / back->getContentSize().width;

	float indent = visibleSize.height / 70.0f * 1.5f;
	float halfIndent = indent / 2.0f;
	widthCard = (size.width - indent * 2.0f) / 2.0f;
	if (widthCard * hightRatio > size.height * 0.6f) widthCard = (size.height * 0.6f) / hightRatio;

	hightCard = hightRatio * widthCard;
	wait = visibleSize.width / 50.0f;

	slotList = new cBaseNode();
	this->addChild(slotList);
	{
		float slotHeight = size.height * 0.3f;
		float slotWidth = (slotHeight / hightRatio);
		if (slotWidth > (size.width - indent * 5.0f) / 4.0f)
		{
			slotWidth = (size.width - indent * 5.0f) / 4.0f;
			slotHeight = slotWidth * hightRatio;
		}
		for (int i = 0; i < 4; i++)
		{
			cocos2d::Sprite *cardSlot = cocos2d::Sprite::create("cards/btm.png");
			cardSlot->setPosition(-(slotWidth + indent) / 2.0f - (slotWidth + indent) * (1 - i), 0);
			cardSlot->setColor(cocos2d::Color3B(0, 0, 0));


			setNodeSize(cardSlot, slotHeight, false, false);
			cardSlot->setOpacity(80);
			slotList->addChild(cardSlot);
			slots.push_back(cardSlot);

			cocos2d::Sprite *icon_plus = cocos2d::Sprite::create("icons/plus.png");
			icon_plus->setOpacity(100);
			icon_plus->setPosition(cardSlot->getContentSize() / 2.0f);
			setNodeSize(icon_plus, cardSlot->getContentSize().width - indent, true, true);
			cardSlot->addChild(icon_plus);

			cButton *btnRemove = new cButton(nullptr, cocos2d::Size(slotWidth * 0.7f, slotWidth * 0.7f), 1.1f, 10);
			btnRemove->setPosition(cardSlot->getContentSize() / 2.0f);
			btnRemove->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
			btnRemove->setLocalZOrder(3);
			btnRemove->setCascadeOpacityEnabled(false);
			for (auto &it : btnRemove->getChildren())
				it->setOpacity(0);

			btnRemove->setTag(i);
			cardSlot->addChild(btnRemove);
			btnRemove->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
			{
				int pos = btnRemove->getTag();
				if (pSelectedCards.size() != 0)
				{
					bool isFound = false;
					for (auto &it : pSelectedCards)
					{
						if (it->pos == pos + 1)
						{
							it->state = -1;
							it->pos = -1;
							isFound = true;
							break;
						}
					}
					if(!isFound)return true;
					currentPos = -1;
					updateStates();
					updateList();
					updateSlots();
				}
				return true;
			};

			cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/exit.png");
			setNodeSize(icon_exit, btnRemove->wnd->getWSize().width * 0.5f, true, true);
			icon_exit->setColor(cocos2d::Color3B(247, 116, 98));
			icon_exit->setOpacity(100);

			icon_exit->setLocalZOrder(4);
			btnRemove->addChild(icon_exit);
			removeBtns.push_back(btnRemove);
		}	
		slotList->setPosition(0, size.height / 2.0f  - indent - size.height * 0.15f);
	}

	cardList = new cBaseNode();
	cardList->setPosition(0, -size.height / 2.0f + indent + size.height * 0.3f);
	this->addChild(cardList);

	//preload
	for(auto &it : pActiveCards)
	{
		it->pCard->getCard(gTtfConfig, 1, 1);
		it->pCard->getCard(gTtfConfig, 0, 1);
	}
	


	//green plus and checkmark
	{
		cocos2d::Size btnAddSize = cocos2d::Size(widthCard * 0.8f, widthCard * 0.8f);
		btnAdd = cocos2d::Sprite::create("icons/plus.png");
		btnAdd->setContentSize(btnAddSize);
		btnAdd->setPosition(0, cardList->getPosition().y);
		btnAdd->setColor(cocos2d::Color3B(70, 200, 70));
		btnAdd->setLocalZOrder(3);
		this->addChild(btnAdd);

		checkmark = cocos2d::Sprite::create("icons/checkmark.png");
		checkmark->setContentSize(btnAddSize);
		checkmark->setPosition(0, cardList->getPosition().y);
		checkmark->setColor(cocos2d::Color3B(70, 200, 70));
		checkmark->setLocalZOrder(3);
		checkmark->setOpacity(200);

		this->addChild(checkmark);
	}

	for (int i = 0; i < 7; i++)
	{
		if (i > (int)pActiveCards.size() - 1)continue;
		cardVisual *el = new cardVisual();
		elements.push_back(el);
	}

	this->listener = cocos2d::EventListenerTouchOneByOne::create();
	this->listener->setSwallowTouches(true);
	this->listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		p = this->convertToNodeSpace(p);

		cocos2d::Rect rect;
		rect.size = cocos2d::Size{ size.width, size.height / 2.5f };
		rect.origin = (size / 2.0f) * -1;
		rect.origin.y = cardList->getPosition().y - rect.size.height / 2.0f;

		if (rect.containsPoint(p))
		{
			isPress = true;
			//updateList();
			pickPoint = p;
			pickPos.y = listPos[1].get();
			pickPos.x = listPos[0].get();
			lastListPos[0] = listPos[0].get();
			return true;
		}
		return false;
	};
	this->listener->onTouchMoved = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		p = this->convertToNodeSpace(p);

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto dS = (p - pickPoint) / 3.0f;
		if (abs(dS.x) > wait)
		{
			if (dS.x > 0) dS.x -= wait;
			else  dS.x += wait;
			listPos[0].set(pickPos.x + dS.x);
		}
		updateList();

		return true;
	};
	this->listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		isPress = false;
		float dP = listPos[0].get() - lastListPos[0];
		if (dP != 0)
		{
			float new_pos = listPos[0].get() + dP * 6.0f;
			listPos[0].set(new_pos, 1.0f, true);
		}		
		cocos2d::Vec2 p = touch->getLocation();
		p = this->convertToNodeSpace(p);
		auto dS = (p - pickPoint) / 3.0f;
		if (abs(dS.x) < wait)
		{
			cocos2d::Rect rect;
			rect.size = btnAdd->getContentSize();
			rect.origin = btnAdd->getPosition() - (btnAdd->getContentSize() / 2.0f);
			if (rect.containsPoint(p) && !listPos[0].isProgress() && btnAdd->isVisible()) //addButon click
			{
				selectCard();				
			}

		}

		return true;
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->listener, 1);

	this->secondUpdate = [&](float dt)
	{
		if (!pActiveCards.empty())
		{
			auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
			float indent = visibleSize.height / 70.0f * 1.5f;

			listPos[0].update(dt); listPos[1].update(dt);
			if (listPos[0].isProgress() || listPos[1].isProgress()) updateList();

			time_points[1] += dt;
			//closer

			if (!isPress )time_points[0] += dt;
			else time_points[0] = 0;
			if (time_points[0] > 0.1f && !listPos[0].isProgress())
			{
				if (elements.at(centerCard) != nullptr && abs(elements.at(centerCard)->originPos.x) > 0.01f)
				{
					updateList();
					listPos[0].set(listPos[0].get() - elements.at(centerCard)->originPos.x, 0.1f);
				}
			}
			if (!isPress && listPos[1].get() != 0 && !listPos[1].isProgress()) 
				listPos[1].set(0, 1.5f, true);

			//inertia
			if (time_points[1] > 0.1f)
			{
				lastListPos[0] = listPos[0].get();
				time_points[1] = 0;
			}

			btnAdd->setVisible(true);
			if (!listPos[0].isProgress() && !isPress)
				btnAdd->setOpacity(200);
			else
				btnAdd->setOpacity(90);

			if (stateCards.at(centerSC).state == -1)
			{
				btnAdd->setVisible(true);
				checkmark->setVisible(false);
				btnAdd->setPosition(elements.at(centerCard)->node->getPosition().x, btnAdd->getPosition().y);
				btnAdd->setRotation3D(elements.at(centerCard)->node->getRotation3D());
			}
			else
			{
				btnAdd->setVisible(false);
				checkmark->setVisible(true);
				checkmark->setPosition(elements.at(centerCard)->node->getPosition().x, btnAdd->getPosition().y);
				checkmark->setRotation3D(elements.at(centerCard)->node->getRotation3D());
			}
		}
		else
		{
			checkmark->setVisible(false);
			btnAdd->setVisible(false);
		}
	};
	updateSlots();
	updateList();
}
cCardInventory::~cCardInventory()
{
}

void cCardInventory::load()
{
	cUserDefault *userDefault = cUserDefault::getInstance();

	stateCards.clear();
	for (auto &it : userDefault->cards)
	{
		cardInfo tmp;
		tmp.pCard = it;
		stateCards.push_back(tmp);
	}

	auto selectedCards = userDefault->profile["aActiveCards"];
	for (auto &it : stateCards)
	{
		for (int i = 0; i < selectedCards.size(); i++)
		{
			auto obj = selectedCards.at(i).begin();
			if (it.pCard->id == obj.key())
			{
				it.pos = obj.value();
				it.state = 1;
				break;
			}
		}
	}
	updateStates();
	updateList();
	updateSlots();
}
void cCardInventory::hide()
{
	if (this->isVisible())
	{
		this->setVisible(false);
		save();
	}
}
void cCardInventory::show()
{
	this->setVisible(true);
}
void cCardInventory::save()
{
	cUserDefault *userDefault = cUserDefault::getInstance();
	auto jsonSaveCards = nlohmann::json::array();
	for (int i = 0; i < pSelectedCards.size(); i++)
	{
		auto obj = nlohmann::json::object();
		obj[pSelectedCards.at(i)->pCard->id] = pSelectedCards.at(i)->pos;
		jsonSaveCards.push_back(obj);
	}
	userDefault->profile["aActiveCards"] = jsonSaveCards;
	userDefault->saveProfile();
}

void cCardInventory::updateList()
{
	if (elements.size() == 0)return;
	cUserDefault *userDefault = cUserDefault::getInstance();
	

	cocos2d::Size elemSize = { size.width / 2.5f, size.height / 2.5f };
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	float indent = visibleSize.height / 70.0f * 1.5f;
	float posScale = (elemSize.width / 3.0f + indent);
	//min max

	float min = -((int)elements.size() / 2 * posScale);
	float max = ((pActiveCards.size() - 1) * posScale) - ((int)elements.size() / 2 * posScale);
	if (listPos[0].get() < min)
	{
		listPos[0].set(min);
	}
	else if (listPos[0].get() > max)
	{
		listPos[0].set(max);
	}
	
	//swap
	int first = ((listPos[0].get()) / posScale);
	int last = ((listPos[0].get() + size.width - posScale) / posScale);

	if (currentPos != first)
	{
		int num_pos = ((listPos[0].get()) / posScale);
		int start_point = 0;
		if (num_pos < 0)
		{
			start_point = num_pos * -1;
			num_pos = 0;
		}
		for (int i = start_point; i < (int)elements.size(); i++)
		{
			if (pActiveCards.size() <= num_pos)continue;

			elements.at(i)->pCard = pActiveCards.at(num_pos)->pCard;
			if (elements.at(i)->node != nullptr)
				elements.at(i)->node->removeFromParent();
			if (elements.at(i)->node != nullptr)
				elements.at(i)->nodeBack->removeFromParent();
			
			elements.at(i)->node = pActiveCards.at(num_pos)->pCard->getCard(userDefault->font, 1, widthCard);
			elements.at(i)->nodeBack = pActiveCards.at(num_pos)->pCard->getCard(userDefault->font, 0, widthCard);
			if (elements.at(i)->originScale == -1)
			{
				elements.at(i)->originSize = elements.at(i)->node->getContentSize() *  elements.at(i)->node->getScale();
				elements.at(i)->originScale = elements.at(i)->node->getScale();
			}
			cardList->addChild(elements.at(i)->node);
			cardList->addChild(elements.at(i)->nodeBack);
			num_pos += 1;
		}
	}
	currentPos = first;

	//orientation
	for (int i = 0; i < (int)elements.size(); i++)
	{
		cocos2d::Vec2 posEl = { (listPos[0].get() - ((first + i) * posScale)) + ((int)elements.size() / 2 * posScale), 0};
		elements.at(i)->originPos = posEl;
		if (centerCard == i) posEl.y = listPos[1].get();

		float proc = abs(posEl.x) / (size.width / 2.0f / 100.0f);
		if (proc > 100.0f)proc = 100.0f; 
		float sc = 3.0f;

		elements.at(i)->node->setScale(elements.at(i)->originScale / (1.0f + (sc / 100.0f) * proc));
		proc = 100.0f - proc;
		elements.at(i)->node->setLocalZOrder(proc);

		float op = (255.0f / 100.0f) * (proc + 20.0f);
		if (op > 255)op = 255;
		else if (op < 0) op = 0;
		elements.at(i)->node->setOpacity(op);

		float rot = (180.0f / 50.0f) * (100.0f - proc);
		if (rot > 180)rot = 180;

		if (rot > 90) { 
			elements.at(i)->node->setVisible(false); elements.at(i)->nodeBack->setVisible(true); }
		else { elements.at(i)->node->setVisible(true); elements.at(i)->nodeBack->setVisible(false); }

		if (posEl.x < 0)rot *= -1;
			elements.at(i)->node->setRotation3D(cocos2d::Vec3(0, rot, 0));
		
		if (
			(first + i) < 0 || 
			(first + i) > (int)pActiveCards.size() - 1 ||
			posEl.x > (size.width / 2.0f) ||
			posEl.x < (-size.width / 2.0f)
			)
		{
			elements.at(i)->node->setPosition(0, visibleSize.height + elemSize.height);
			elements.at(i)->nodeBack->setPosition(0, visibleSize.height + elemSize.height);

			elements.at(i)->node->setVisible(false);// = cocos2d::Vec2(0, visibleSize.height + elemSize.height);
			elements.at(i)->nodeBack->setVisible(false);
			continue;
		}
		if (abs(elements.at(centerCard)->originPos.x) > abs(elements.at(i)->originPos.x) && elements.at(i)->node->isVisible() == true)
		{
			listPos[1].set(0);
			for (int j = 0; j < (int)stateCards.size(); j++)
				if (elements.at(i)->pCard == stateCards.at(j).pCard) 
				{
					centerSC = j;
					break;
				}
			centerCard = i;
		}
		elements.at(i)->node->setPosition(posEl);

		elements.at(i)->nodeBack->setPosition(posEl);
		elements.at(i)->nodeBack->setScale(elements.at(i)->node->getScale());
		elements.at(i)->nodeBack->setRotation3D(cocos2d::Vec3(0,elements.at(i)->node->getRotation3D().y + 180,0));
		elements.at(i)->nodeBack->setLocalZOrder(elements.at(i)->node->getLocalZOrder());
		elements.at(i)->nodeBack->setOpacity(elements.at(i)->node->getOpacity());

		elements.at(i)->originPos = posEl;		
	}
}
void cCardInventory::updateSlots()
{
	cUserDefault *userDefault = cUserDefault::getInstance();
	for (int i = 0; i < slots.size(); i++)
	{
		auto node = slots.at(i)->getChildByName("card");
		if(node != nullptr)node->removeFromParent();
	}

	for (int i = 0; i < slots.size(); i++)
	{
		if (i > (int)pSelectedCards.size() - 1) break;

		auto node = pSelectedCards.at(i)->pCard->getCard(userDefault->font, 1, slots.at(pSelectedCards.at(i)->pos - 1)->getContentSize().width);
		node->setName("card");
		node->setPosition(slots.at(pSelectedCards.at(i)->pos - 1)->getContentSize() / 2.0f);
		slots.at(pSelectedCards.at(i)->pos - 1)->addChild(node);
	}

	for (int i = 0; i < (int)slots.size(); i++)
	{
		if (slots.at(i)->getChildByName("card"))removeBtns.at(i)->setVisible(true);
		else removeBtns.at(i)->setVisible(false);
	}
}
void cCardInventory::updateStates()
{
	pActiveCards.clear();
	pSelectedCards.clear();
	for (auto &it : stateCards)
	{
		pActiveCards.push_back(&it);
		if (it.state == 1)pSelectedCards.push_back(&it);
	}
}

void cCardInventory::selectCard()
{
	int newCardPos = 1;
	if (pSelectedCards.size() == 4)
	{
		for (int i = 0; i < pSelectedCards.size(); i++)
		{
			if (pSelectedCards.at(i)->pos == 1) { pSelectedCards.at(i)->state = -1; pSelectedCards.at(i)->pos = -1; }
			else pSelectedCards.at(i)->pos -= 1;
		}
		newCardPos = 4;
	}
	else
	{
		for (int i = 0; i < pSelectedCards.size(); i++)
		{
			if (pSelectedCards.at(i)->pos == newCardPos)
			{
				newCardPos++;
				i = -1;
			}
		}
	}

	for (int i = 0; i < stateCards.size(); i++)
	{
		if (elements.at(centerCard)->pCard == stateCards.at(i).pCard)
		{
			stateCards.at(i).state = 1;
			stateCards.at(i).pos = newCardPos;
			break;
		}
	}
	currentPos = -199999999999;
	updateStates();
	updateList();
	updateSlots();
}
