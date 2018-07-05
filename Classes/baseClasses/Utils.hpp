#ifndef UTILS_HPP
#define UTILS_HPP

#include "cocos2d.h"
#include "Box2D/Box2D.h"


void setNodeSize(cocos2d::Node* pNode, float size, bool bScale, bool sideWidth);
b2Body * createColision(b2World *m_world, b2BodyType flag, std::vector<b2Vec2> vertices);
b2Body * createColision(b2World *m_world, b2BodyType flag, float radius);

void setNodeInRect(cocos2d::Node* pNode, cocos2d::Size sz, float indent);

std::vector<std::string> Split(std::string src, char sep);

#endif // !UTILS_HPP

