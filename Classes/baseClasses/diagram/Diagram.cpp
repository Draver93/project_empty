#include "Diagram.hpp"

cDiagram::cDiagram(std::vector<float> *data, float width)
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	this->setCascadeOpacityEnabled(true);
	//this->init(dSize, W_MUTE, 0);
	float maxVal = 0;
	for (int i = 0; i < data->size(); i++)
		if (maxVal < data->at(i))maxVal = data->at(i);

	back = cocos2d::DrawNode::create();
	back->setLineWidth(visibleSize.width / 100.0f);
	this->addChild(back);
	poly = cocos2d::DrawNode::create();
	this->addChild(poly);

	float sizeDiagram = (10 * (data->size() + 1)) * 2.0f;

	std::vector<cocos2d::Vec2> polyData;
	for (int i = 0; i < data->size(); i++)
	{
		float rad = (360 / data->size() * i + (360 / data->size() / 2.0f)) * (3.14f / 180);
		cocos2d::Vec2 dest = {0,0};
		dest.x += (10 * (data->size())) * sin(rad);
		dest.y += (10 * (data->size())) * cos(rad);

		cocos2d::Vec2 point = { 0,0 };

		point.x += (10 * data->size() / maxVal * data->at(i)) * sin(rad);
		point.y += (10 * data->size() / maxVal * data->at(i)) * cos(rad);
		polyData.push_back(point);
		back->drawLine(cocos2d::Vec2(0, 0), dest, cocos2d::Color4F(1, 1, 1, 1));
	}
	for (int i = 0; i < 4; i++)
	{
		back->drawCircle(cocos2d::Vec2(0, 0), sizeDiagram / 9.0f * (i + 1), 1, 100, false, cocos2d::Color4F(1, 1, 1, 1));
	}
	for (int i = 0; i < polyData.size(); i++)
	{
		std::vector<cocos2d::Vec2> tmpData = { {0, 0}, polyData.at(i) };
		if (i + 1 > polyData.size() - 1)tmpData.push_back(polyData.at(0));
		else tmpData.push_back(polyData.at(i + 1)); 
		poly->drawSolidPoly(tmpData.data(), tmpData.size(), cocos2d::Color4F(247.0f / 255.0f, 116.0f / 255.0f, 98.0f / 255.0f, 1.0f));
	}
	back->setScale((width / sizeDiagram) * 0.9f);
	poly->setScale((width / sizeDiagram) * 0.9f);

	//this->setScale(2);
}

cDiagram::~cDiagram()
{
}

void cDiagram::update(float dt)
{
}
