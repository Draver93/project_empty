#include "TeacherCard.hpp"



cTeacherCard::cTeacherCard(std::string cardId, std::string data)
{
	std::vector<std::string> vecStr = Split(data, '|');

	id = cardId;
	name = vecStr.at(0);
	filePath = "cards/" + vecStr.at(1);
	rare = std::stoi(vecStr.at(3));
	std::vector<std::string> subjectVec = Split(vecStr.at(2), ':');
	//coment = cardComent;

	subject = std::stoi(subjectVec.at(0));
	stat = std::stof(subjectVec.at(1));
}
cTeacherCard::~cTeacherCard()
{
}
cocos2d::Node * cTeacherCard::getCard(cocos2d::TTFConfig *gTtfC, bool side, float width)
{
	gTtfConfig = gTtfC;
	if (side == 0) return this->getCardBack(rare, width);
	else return this->getCardFront(gTtfConfig, width, name, filePath, subject, stat, rare);
}
void cTeacherCard::setupGameField(cGameField * field)
{
}
