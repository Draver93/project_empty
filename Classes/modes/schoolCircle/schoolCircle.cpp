#include "schoolCircle.hpp"

cSchoolCircle::cSchoolCircle(std::string data)
{
}

cSchoolCircle::~cSchoolCircle()
{
}

cDinamicLayer * cSchoolCircle::getNode(int type, cocos2d::Size size)
{
	switch (type)
	{
	case 0: return new cSCIntro(size, this);
		break;
	case 1: return new cSCGame(size, this);
		break;
	case 2: return new cSCOutro(size, this);
		break;
	default:
		break;
	}
	return nullptr;
}

std::string cSchoolCircle::generateMode()
{
	return std::string();
}
