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

	virtual cDinamicLayer* getModeIntro(cocos2d::Size size) = 0;
	virtual cDinamicLayer* getGameNode(cocos2d::Size size) = 0;
	virtual cDinamicLayer* getModeOutro(cocos2d::Size size) = 0;
	virtual std::string generateMode(std::string modeName) = 0;
};


#endif //BASE_MODE_HPP