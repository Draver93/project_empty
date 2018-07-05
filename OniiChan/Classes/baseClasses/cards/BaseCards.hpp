#ifndef BASECARDS_HPP
#define BASECARDS_HPP

#include <string>
#include <vector>
#include "cocos2d.h"

#include "baseClasses/ui/UI.hpp"
#include "baseClasses/BaseNode.hpp"
#include "baseClasses/game/GameField.hpp"


class cCard
{
public:
	std::string id;
	std::string name;
	std::string filePath;
	int subject;
	float stat;
protected:
	std::string coment;
	int rare;
public:
	cCard();
	~cCard();

	static cocos2d::Node* getCardFront(cocos2d::TTFConfig *gTtfConfig, float width, std::string name, std::string path, int subject, float stat, int rare);
	static cocos2d::Node* getCardBack(int rare, float width);
	virtual cocos2d::Node* getCard(cocos2d::TTFConfig *gTtfConfig, bool side, float width) = 0;
};




#endif // !BASECARDS_HPP
