#ifndef REST_HPP
#define REST_HPP

#include "baseClasses/UserDefault.hpp"
#include "baseClasses/BaseMode.hpp"
#include "baseClasses/DinamicLayer.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/Utils.hpp"
#include "baseClasses/game/GameField.hpp"
#include "baseClasses/dialogs/Dialog.hpp"
#include "cards/Cards.hpp"

#include "rGame.hpp"
#include "rIntro.hpp"
#include "rOutro.hpp"


class cRest : public cBaseMode
{
public:

public:
	cRest(std::string data);
	~cRest();
	cDinamicLayer* getNode(int type, cocos2d::Size size);
	static std::string generateMode();
	std::string getModeName() { return u8"Экзамен"; };

};

#endif // !REST_HPP
