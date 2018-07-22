#ifndef SCHOOL_CIRCLE_HPP
#define SCHOOL_CIRCLE_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

#include "scGame.hpp"
#include "scIntro.hpp"
#include "scOutro.hpp"

class cSchoolCircle : public cBaseMode
{
public:

public:
	cSchoolCircle(std::string data);
	~cSchoolCircle();
	cDinamicLayer* getNode(int type, cocos2d::Size size);
	static std::string generateMode();
	std::string getModeName() { return u8"Экзамен"; };

};

#endif // !VERIFICATION_WORK_HPP
