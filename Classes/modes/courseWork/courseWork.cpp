#include "courseWork.hpp"

cCourseWork::cCourseWork(std::string data)
{
}

cCourseWork::~cCourseWork()
{
}

cDinamicLayer * cCourseWork::getNode(int type, cocos2d::Size size)
{
	switch (type)
	{
	case 0: return new cCWIntro(size, this);
		break;
	case 1: return new cCWGame(size, this);
		break;
	case 2: return new cCWOutro(size, this);
		break;
	default:
		break;
	}
	return nullptr;
}

std::string cCourseWork::generateMode()
{
	return std::string();
}
