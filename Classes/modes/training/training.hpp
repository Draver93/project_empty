#ifndef TRAINING_HPP
#define TRAINING_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

#include "tGame.hpp"
#include "tIntro.hpp"
#include "tOutro.hpp"


class cTraining : public cBaseMode
{
public:

public:
	cTraining(std::string data);
	~cTraining();
	cDinamicLayer* getNode(int type, cocos2d::Size size);
	static std::string generateMode();
	std::string getModeName() { return u8"�������"; };

};

#endif // !TRAINING_HPP
