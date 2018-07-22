#ifndef INDEPENDENT_WORK_HPP
#define INDEPENDENT_WORK_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

#include "iwGame.hpp"
#include "iwIntro.hpp"
#include "iwOutro.hpp"


class cIndependentWork : public cBaseMode
{
public:

public:
	cIndependentWork(std::string data);
	~cIndependentWork();
	cDinamicLayer* getNode(int type, cocos2d::Size size);
	static std::string generateMode();
	std::string getModeName() { return u8"Экзамен"; };

};

#endif // !INDEPENDENT_WORK_HPP
