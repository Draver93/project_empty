#ifndef STUDENT_CARD_HPP
#define STUDENT_CARD_HPP

#include <string>
#include <vector>
#include "cocos2d.h"

#include "baseClasses/ui/UI.hpp"
#include "baseClasses/BaseNode.hpp"
#include "baseClasses/cards/BaseCards.hpp"


class cStudentCard : public cCard
{
public:
	std::string rData;
private:
	cocos2d::TTFConfig *gTtfConfig;
public:
	cStudentCard(std::string cardId, std::string data);
	~cStudentCard();

	cocos2d::Node* getCard(cocos2d::TTFConfig *gTtfC, bool side, float width);
	void setupGameField(cGameField *field);

};


#endif // !STUDENT_CARD_HPP
