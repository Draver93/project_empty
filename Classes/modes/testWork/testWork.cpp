#include "testWork.hpp"

cTestWork::cTestWork(std::string data)
{
}

cTestWork::~cTestWork()
{
}

cDinamicLayer * cTestWork::getNode(int type, cocos2d::Size size)
{
	switch (type)
	{
	case 0: return new cTWIntro(size, this);
		break;
	case 1: return new cTWGame(size, this);
		break;
	case 2: return new cTWOutro(size, this);
		break;
	default:
		break;
	}
	return nullptr;
}

std::string cTestWork::generateMode()
{
	return std::string();
}
