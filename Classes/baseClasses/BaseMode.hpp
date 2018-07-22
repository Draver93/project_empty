#ifndef BASE_MODE_HPP
#define BASE_MODE_HPP

#include "cocos2d.h"
#include "baseClasses/Utils.hpp"
#include "baseClasses/DinamicLayer.hpp"

class cBaseMode
{
public:
	cBaseMode();
	~cBaseMode();

	virtual cDinamicLayer* getNode(int type, cocos2d::Size size) = 0;
	virtual std::string getModeName() = 0;

};


#endif //BASE_MODE_HPP