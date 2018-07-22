#ifndef VERIFICATION_WORK_HPP
#define VERIFICATION_WORK_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

#include "vwGame.hpp"
#include "vwIntro.hpp"
#include "vwOutro.hpp"

class cVerificationWork : public cBaseMode
{
public:
	std::vector<int> subjects;
	std::vector<float> skills;
	float currentScore;
	float turns;
public:
	cVerificationWork(std::string data);
	~cVerificationWork();
	cDinamicLayer* getNode(int type, cocos2d::Size size);
	static std::string generateMode();
	std::string getModeName() { return u8"Экзамен"; };
};

#endif // !VERIFICATION_WORK_HPP
