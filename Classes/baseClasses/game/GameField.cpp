#include "GameField.hpp"

void cGameField::createBorders(b2World * world, cocos2d::Vec2 nSize)
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	float scaleH = visibleSize.height / visibleSize.width;
	updateTime = 0;
	//physicalField
	{
		size = nSize;
		float rad = (1 * scalePhysicalWorld) / 2.0f;

		bool idt = true;
		b2Vec2 vecBegin = { 0,0 }, vecEnd = { 0,0 };
		for (int i = 0; i < nSize.x - 2; i++)
		{
			if (i == 0)
			{
				fieldWH = { rad * 2.0f , rad };
				vecBegin = fieldWH;
			}
			else
			{
				float angle = 30;
				if (idt)angle = -30;

				fieldWH.x += (rad * 2.0f) * cos(angle * (3.14f / 180));
				fieldWH.y += (rad * 2.0f) * sin(angle * (3.14f / 180));
				idt = !idt;
			}
		}


		b2Body *body = nullptr;
		//Bottom true;
		idt = true;
		b2Vec2 posFirst = { 0, 0 }, posLast = { 0, 0 };
		for (int i = 0; i < nSize.x; i++)
		{
			b2Vec2 pos = { 0,0 };
			if (body != nullptr)
			{
				b2Vec2 lastBPos = body->GetPosition();
				pos = lastBPos;

				float angle = 30;
				if (idt)angle = -30;

				pos.x += (rad * 2.0f) * cos(angle * (3.14f / 180));
				pos.y += (rad * 2.0f) * sin(angle * (3.14f / 180));
				idt = !idt;
			}
			else
			{
				pos = { rad , rad };
				pos.x += (rad * 2.0f) * cos(210 * (3.14f / 180));
				pos.y += (rad * 2.0f) * sin(210 * (3.14f / 180));
			}
			body = createColision(world, b2_staticBody, rad);
			body->SetTransform(pos, 0);
			if (i == 1)
			{
				posFirst = body->GetPosition();
				posFirst.y += rad * 2.0f;
			}
			else if (i == nSize.x - 2)
			{
				posLast = body->GetPosition();
				posLast.y += rad * 2.0f;
			}
		}
		float left = posFirst.x - rad * 2.0f;
		float right = posLast.x + rad * 2.0f;
		sizeField.x = fabs(left) + right;
		sizeField.y = (sizeField.x * scaleH) * 2.0f;
		std::vector<b2Vec2> vertices;
		vertices.push_back( b2Vec2(rad, sizeField.y / 2.0f) );
		vertices.push_back( b2Vec2(-rad, sizeField.y / 2.0f) );
		vertices.push_back( b2Vec2(-rad, -sizeField.y / 2.0f) );
		vertices.push_back( b2Vec2(rad, -sizeField.y / 2.0f) );

		b2Body* leftBorder = createColision(world, b2_staticBody, vertices);
		b2Body* rightBorder = createColision(world, b2_staticBody, vertices);
		leftBorder->SetTransform(b2Vec2( posFirst.x - rad * 2.0f, sizeField.y / 2.0f), 0);
		rightBorder->SetTransform(b2Vec2( posLast.x + rad * 2.0f, sizeField.y / 2.0f), 0);

		vertices.clear();
		vertices.push_back(b2Vec2(sizeField.x / 2.0f, rad));
		vertices.push_back(b2Vec2(-sizeField.x / 2.0f, rad));
		vertices.push_back(b2Vec2(-sizeField.x / 2.0f, -rad));
		vertices.push_back(b2Vec2(sizeField.x / 2.0f, -rad));

		b2Body* topBorder = createColision(world, b2_staticBody, vertices);
		topBorder->SetTransform(b2Vec2(sizeField.x / 2.0f, sizeField.y), 0);

	}

	//Listener
	{
		listener = cocos2d::EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
		{
			pickElement(touch);
			return true;
		};
		listener->onTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Event* event)
		{
			pickElement(touch);
		};
		listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
		{
			if (pickElements.size() > 2)
			{
				turnCount += 1;
				lastPickLine.clear();
				for (unsigned int i = 0; i < pickElements.size(); i++)
					lastPickLine.push_back(pickElements.at(i)->type);

				for (unsigned int i = 0; i < pickElements.size(); i++)
					for (unsigned int j = 0; j < arrayElements.size(); j++)
					{
						if (pickElements.at(i) == arrayElements.at(j))
						{
							world->DestroyBody(arrayElements.at(j)->body);
							this->removeChild(arrayElements.at(j)->pSprite, true);
							delete arrayElements.at(j);
							arrayElements.erase(arrayElements.begin() + j);
							break;
						}
					}
			}
			else for (unsigned int i = 0; i < pickElements.size(); i++)
			{
				changeSize(pickElements.at(i), (1 * scalePhysicalWorld) / 2.0f);
				//pickElements.at(i)->rotate = false;
				pickElements.at(i)->pSprite->setColor(cocos2d::Color3B(255, 255, 255));
			}
			pickElements.clear();
		};

		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
	}
}
bool cGameField::checkExist(sElement * el)
{
	for (unsigned int j = 0; j < pickElements.size(); j++)
		if (pickElements.at(j) == el)return true;
	return false;
}
bool cGameField::checkDistance(sElement * el, b2Vec2 touchPos, float rad)
{
	bool accept = false;
	cocos2d::Vec2 posElementA = this->convertToWorldSpace(el->pSprite->getPosition());
	float dist = b2Distance(b2Vec2(posElementA.x, posElementA.y), touchPos);

	if (dist < rad) 
	{
		accept = true;
	}
	return accept;
}
void cGameField::changeSize(sElement *el, float size)
{
	return;
	float scl = widthField / fieldWH.x;

	b2Fixture *fixture = el->body->GetFixtureList();
	el->body->DestroyFixture(fixture);
	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0); //position, relative to body position
	circleShape.m_radius = size;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape; //change the shape of the fixture
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 10.0f;
	fixtureDef.restitution = 0.1f;

	el->body->CreateFixture(&fixtureDef); //add a fixture to the body
	el->pSprite->setContentSize(cocos2d::Size(size * scl * 2.0f, size * scl* 2.0f));

}
void cGameField::pickElement(cocos2d::Touch* touch)
{
	cocos2d::Vec2 p = touch->getLocation();
	float scl = widthField / fieldWH.x;
	float rad = 0; //= (1 * scalePhysicalWorld) / 2.0f * scl;
	if(!arrayElements.empty())
	{
		rad = cocos2d::utils::getCascadeBoundingBox(arrayElements.at(0)->pSprite).size.width / 2.0f;
	}
	else return;

	for (unsigned int i = 0; i < arrayElements.size(); i++)
	{
		if (checkDistance(arrayElements.at(i), b2Vec2(p.x, p.y), rad))
		{
			if (pickElements.size() > 1 && pickElements.at(pickElements.size() - 2) == arrayElements.at(i))
			{
				changeSize(pickElements.back(), (1 * scalePhysicalWorld) / 2.0f);
				pickElements.back()->pSprite->setColor(cocos2d::Color3B(255, 255, 255));
				//pickElements.back()->rotate = false;
				pickElements.pop_back();
				changeSize(pickElements.back(), (1 * scalePhysicalWorld) / 2.7f);
				break;
			}
			if (!checkExist(arrayElements.at(i)))
			{
				if (pickElements.empty())
				{
					pickElements.push_back(arrayElements.at(i));
					arrayElements.at(i)->body->ApplyLinearImpulse(b2Vec2(0, 20), arrayElements.at(i)->body->GetWorldCenter(), true);
					//arrayElements.at(i)->rotate = true;
					arrayElements.at(i)->pSprite->setColor(cocos2d::Color3B(50, 50, 50));
					changeSize(pickElements.back(), (1 * scalePhysicalWorld) / 2.7f);

				}
				else if (pickElements.back()->type == arrayElements.at(i)->type)
				{
					cocos2d::Vec2 posElementB = this->convertToWorldSpace(pickElements.back()->pSprite->getPosition());
					if (checkDistance(arrayElements.at(i), b2Vec2(posElementB.x, posElementB.y), rad * 2.2f))
					{
						//changeSize(pickElements.back(), (1 * scalePhysicalWorld) / 2.0f);
						pickElements.push_back(arrayElements.at(i));
						arrayElements.at(i)->pSprite->setColor(cocos2d::Color3B(50, 50, 50));
						//arrayElements.at(i)->rotate = true;
						//changeSize(pickElements.back(), (1 * scalePhysicalWorld) / 2.7f);
					}
				}
			}
		}
	}
}

cGameField::cGameField(float width, cocos2d::Vec2 size)
{
	widthField = width;
	turnCount = 0;
	lastPickLine.clear();
	scalePhysicalWorld = 1;
	b2Vec2 gravity(0, -50.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
	createBorders(world, size);

	secondUpdate = [=](float dt)
	{
		if (dt > 1.0f / 60.0f) world->Step(1.0f / 60.0f, 50, 5);
		else world->Step(dt, 50, 5);
		float scl = widthField / fieldWH.x;
		for (unsigned int i = 0; i < arrayElements.size(); i++)
		{
			b2Vec2 pos = arrayElements.at(i)->body->GetPosition();
			arrayElements.at(i)->pSprite->setPosition(pos.x * scl, pos.y * scl);
		}
	};
}

cGameField::~cGameField()
{
	delete world;
}


sElement* cGameField::addElement(int type, b2BodyType bType)
{
	float scl = widthField / fieldWH.x;

	float rad = (1 * scalePhysicalWorld) / 2.0f;

	sElement *element = new sElement();

	element->type = type;
	element->body = createColision(world, bType, rad);

	int countPos = 10;
	int posRand = rand() % countPos;
	element->body->SetTransform(b2Vec2((sizeField.x / (countPos + 1.0f)) * (posRand + 1) - rad, sizeField.y * 0.6f), 0);

	std::string fileName = "gameScreen/" + std::to_string(type) + ".png";


	element->pSprite = cocos2d::Sprite::create(fileName);
	this->addChild(element->pSprite);

	element->pSprite->setContentSize(cocos2d::Size(rad * scl * 2.0f, rad * scl * 2.0f));

	arrayElements.push_back(element);
	
	b2Vec2 pos = element->body->GetPosition();
	element->pSprite->setPosition(pos.x * scl, pos.y * scl);

	return element;
}

std::vector<sElement*> cGameField::getElements()
{
	return arrayElements;
}

void cGameField::deleteElement(b2Body * type)
{
}
