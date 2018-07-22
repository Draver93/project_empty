#include "independentWork.hpp"

cIndependentWork::cIndependentWork(std::string data)
{
}

cIndependentWork::~cIndependentWork()
{
}

cDinamicLayer * cIndependentWork::getNode(int type, cocos2d::Size size)
{
	switch (type)
	{
	case 0: return new cIWIntro(size, this);
		break;
	case 1: return new cIWGame(size, this);
		break;
	case 2: return new cIWOutro(size, this);
		break;
	default:
		break;
	}
	return nullptr;
}

std::string cIndependentWork::generateMode()
{
	return std::string();
}
