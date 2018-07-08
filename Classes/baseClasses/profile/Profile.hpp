#ifndef PROFILE_HPP
#define PROFILE_HPP

#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseNode.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/diagram/Diagram.hpp"
#include "baseClasses/UserDefault.hpp"



class cProfile : public cBaseNode
{
private:
	int activeZone;
	cocos2d::Vec2 startPoint, endPoint;

	cocos2d::Size size;
	cDiagram *diagram;
	cocos2d::Label* profileAvScore;
	std::vector<cocos2d::Label*> profileLevel;
	std::vector<float> profileSkills;

	struct statsData 
	{
		float stAngle, dtAngle;
		float statAngle, changeAngle;
		cocos2d::Size size;
		cocos2d::Vec2 pos;
		bool direct;
		~statsData()
		{
			direct = false;
			statAngle = changeAngle = 0;
		}

	} stats;
	struct avaData
	{
		float stAngle;

	} avatarData;

	int statElementShowId;
	cDigit statVal;
	cocos2d::Label *statValLabel;

	std::vector<floatElement*> infoElements;

	cocos2d::Label *aScore, *bScore, *lScore;

public:
	bool firstStart = false;
	cProfile(cocos2d::Size sizeProfile);
	~cProfile();
	void updateStats();
	void hide();
	void show();
};

#endif // !PROFILE_HPP
