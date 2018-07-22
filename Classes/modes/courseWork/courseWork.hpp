#ifndef COURSE_WORK_HPP
#define COURSE_WORK_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

#include "cwGame.hpp"
#include "cwIntro.hpp"
#include "cwOutro.hpp"


class cCourseWork : public cBaseMode
{
public:

public:
	cCourseWork(std::string data);
	~cCourseWork();
	cDinamicLayer* getNode(int type, cocos2d::Size size);
	static std::string generateMode();
	std::string getModeName() { return u8"Экзамен"; };

};

#endif // !COURSE_WORK_HPP
