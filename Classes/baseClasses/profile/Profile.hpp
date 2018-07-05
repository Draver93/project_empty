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

	struct infoElevent {
		cBaseNode *node;
		cocos2d::Vec2 originPos;
		cDigit x, y;
		cDigit value;

		infoElevent()
		{
		};
		~infoElevent()
		{
		};
		void update(float dt, float indent)
		{
			x.update(dt);
			y.update(dt);
			if (!x.isProgress())
			{
				int proc = rand() % 100;
				float ind = (indent / 100.0f) * proc - indent / 2.0f;

				x.set(ind, (rand() % 4) + 1);
			}
			if (!y.isProgress())
			{
				int proc = rand() % 100;
				float ind = (indent / 100.0f) * proc - indent / 2.0f;

				y.set(ind, (rand() % 4) + 1);
			}
			node->setPosition(originPos.x + x.get(), originPos.y + y.get());
		};
	};
	std::vector<infoElevent*> infoElements;

public:
	bool firstStart = false;
	cProfile(cocos2d::Size sizeProfile);
	~cProfile();
};

#endif // !PROFILE_HPP
