#include "Utils.hpp"

void setNodeSize(cocos2d::Node* pNode, float size, bool bScale, bool sideWidth)
{
	if (pNode == nullptr && size > 0.0f) return;
	cocos2d::Size labelSize = pNode->getContentSize();
	float scale;
	if (!sideWidth) scale = size / labelSize.height;
	else  scale = size / labelSize.width;

	if (bScale) pNode->setScale(scale, scale);
	else
	{
		labelSize.width *= scale;
		labelSize.height *= scale;
		pNode->setContentSize(labelSize);
	}
}
b2Body * createColision(b2World *m_world, b2BodyType flag, std::vector<b2Vec2> vertices)
{
	b2BodyDef bodyDef;
	bodyDef.linearDamping = 0.0f;
	bodyDef.type = flag;
	b2Body* Body = m_world->CreateBody(&bodyDef);

	b2PolygonShape polygonShape;
	polygonShape.Set(vertices.data(), vertices.size()); //pass array to the shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape; //change the shape of the fixture
	fixtureDef.friction = 0.0f;
	//fixtureDef.density = 0.2f;
	Body->CreateFixture(&fixtureDef); //add a fixture to the body
	return Body;
}
b2Body * createColision(b2World *m_world, b2BodyType flag, float radius)
{
	b2BodyDef bodyDef;
	bodyDef.linearDamping = 0.5f;
	bodyDef.type = flag;
	b2Body* Body = m_world->CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0); //position, relative to body position
	circleShape.m_radius = radius; //radius
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape; //change the shape of the fixture
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 1.3f;
	fixtureDef.restitution = 0.0f;
	Body->CreateFixture(&fixtureDef); //add a fixture to the body
	Body->SetFixedRotation(true);
	return Body;
}

void setNodeInRect(cocos2d::Node * pNode, cocos2d::Size sz, float indent)
{
	setNodeSize(pNode, sz.height - indent, true, false);
	cocos2d::Size szNew = pNode->getContentSize() * pNode->getScale();
	if (szNew.width - (sz.width - indent) > 0) setNodeSize(pNode, sz.width - indent, true, true);
}

std::vector<std::string> Split(std::string src, char sep)
{
	std::vector<std::string> resultVec;
	while (!src.empty())
	{
		std::string tmpStr;
		for (unsigned int i = 0; i < src.size(); i++)
		{
			char it = src.at(i);
			if (it == sep || i + 1 == src.size())
			{
				if (i + 1 == src.size())
					tmpStr.push_back(it);
				if(tmpStr.size() != 0)
					resultVec.push_back(tmpStr);

				if (i + 1 >= src.size())src.clear();
				else src = src.substr(i + 1, src.size());
				break;
			}
			else tmpStr.push_back(it);
		}
	}

	return resultVec;
}
