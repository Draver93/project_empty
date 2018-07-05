#ifndef DIAGRAM_HPP
#define DIAGRAM_HPP


#include "cocos2d.h"
#include "string"
#include "vector"
#include "baseClasses/ui/UI.hpp"

//Radar chart
class cDiagram : public cBaseNode
{
private:
	cocos2d::DrawNode *back;
	cocos2d::DrawNode *poly;

	std::vector<int> arrayData;
public:
	cDiagram(std::vector<float> *data, float width);
	~cDiagram();

	void update(float dt) override;
};

#endif // !DIAGRAM_HPP
