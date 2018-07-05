#ifndef MM_COUNT_HPP
#define MM_COUNT_HPP

#include "baseClasses/BaseMode.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/UserDefault.hpp"

class cMMCount : public cBaseMode
{
private:

public:
	cMMCount();
	~cMMCount();
	cDinamicLayer* getModeIntro(cocos2d::Size size);
	cDinamicLayer* getGameNode(cocos2d::Size size);
	cDinamicLayer* getModeOutro(cocos2d::Size size);
	std::string generateMode(std::string modeName);
};

#endif // !MM_COUNT_HPP
