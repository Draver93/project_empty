#include "training.hpp"

cTraining::cTraining(std::string data)
{
}

cTraining::~cTraining()
{
}

cDinamicLayer * cTraining::getNode(int type, cocos2d::Size size)
{
	switch (type)
	{
	case 0: return new cTIntro(size, this);
		break;
	case 1: return new cTGame(size, this);
		break;
	case 2: return new cTOutro(size, this);
		break;
	default:
		break;
	}
	return nullptr;
}

std::string cTraining::generateMode()
{
	return std::string();
}
