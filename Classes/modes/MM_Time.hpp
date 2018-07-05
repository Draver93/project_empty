#ifndef MM_TIME_HPP
#define MM_TIME_HPP

#include "baseClasses/BaseMode.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/UserDefault.hpp"

class cMMTime : public cBaseMode
{
private:

public:
	cMMTime();
	~cMMTime();
	cDinamicLayer* getModeIntro(cocos2d::Size size);
	cDinamicLayer* getGameNode(cocos2d::Size size);
	cDinamicLayer* getModeOutro(cocos2d::Size size);
	std::string generateMode(std::string modeName);

};

#endif MM_TIME_HPP