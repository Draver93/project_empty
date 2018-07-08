#ifndef DINAMIC_LAYER_HPP
#define DINAMIC_LAYER_HPP

#include "cocos2d.h"
#include "string"
#include "vector"
#include "baseClasses/digitEx/cDigit.hpp"

class cDinamicLayer : public cocos2d::Layer
{
private:
	float time;
public:
	cDinamicLayer();
	~cDinamicLayer();

	virtual void step(float dt) = 0;
	virtual void reset() = 0;
};


#endif //DINAMIC_LAYER_HPP