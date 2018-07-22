#ifndef GAME_FIELD_HPP
#define GAME_FIELD_HPP

#include <vector>

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseNode.hpp"

static std::vector<float> incr = { 1.0f, 1.0f, 1.0f, 1.2f, 1.5f ,2.0f ,3.0f, 4.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f };

struct sElement
{
	bool rotate = false;
	cocos2d::Sprite *pSprite;
	int type;
	b2Body *body;
};

class cGameField : public cBaseNode
{
private:
	float updateTime;
	cocos2d::Vec2 size;
	float widthField;
	float scalePhysicalWorld;
	b2World *world;

	std::vector<sElement*> arrayElements;
	std::vector<sElement*> pickElements;
	cocos2d::Vec2 sizeField;
	std::vector<int> lastPickLine;
	int turnCount;


	void createBorders(b2World *world, cocos2d::Vec2 size);
	bool checkExist(sElement* el);
	bool checkDistance(sElement* el, b2Vec2 touchPos, float rad);
	void changeSize(sElement *el, float size);
	void pickElement(cocos2d::Touch* touch);
public:
	b2Vec2 fieldWH;
	cGameField(float width, cocos2d::Vec2 size);
	~cGameField();

	sElement* addElement(int type, b2BodyType bType = b2_dynamicBody);
	std::vector<sElement*> getElements();
	void deleteElement(int pos);

	std::vector<int> getLastLine() { return lastPickLine; };
	int getTurnCount() { return turnCount; }
};

#endif // !GAME_FIELD_HPP
