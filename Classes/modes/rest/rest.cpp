#include "rest.hpp"

cRest::cRest(std::string data)
{
}

cRest::~cRest()
{
}

cDinamicLayer * cRest::getNode(int type, cocos2d::Size size)
{
	switch (type)
	{
	case 0: return new cRIntro(size, this);
		break;
	case 1: return new cRGame(size, this);
		break;
	case 2: return new cROutro(size, this);
		break;
	default:
		break;
	}
	return nullptr;
}

std::string cRest::generateMode()
{
	return std::string();
}
