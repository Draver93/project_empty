#ifndef TEACHER_CARD_HPP
#define TEACHER_CARD_HPP

#include <string>
#include <vector>
#include "cocos2d.h"

#include "baseClasses/ui/UI.hpp"
#include "baseClasses/BaseNode.hpp"
#include "baseClasses/cards/BaseCards.hpp"


class cTeacherCard : public cCard
{
private:
	cocos2d::TTFConfig *gTtfConfig;
public:
	cTeacherCard(std::string cardId, std::string data);
	~cTeacherCard();

	cocos2d::Node* getCard(cocos2d::TTFConfig *gTtfC, bool side, float width);
	void setupGameField(cGameField *field);
};

#endif // !TEACHER_CARD_HPP
