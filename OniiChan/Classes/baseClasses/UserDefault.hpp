#ifndef USER_DEFAULT_HPP
#define USER_DEFAULT_HPP

#include <json/rapidjson.h>
#include <json/document.h>

#include <json/writer.h>
#include <json/istreamwrapper.h>
#include <json/ostreamwrapper.h>
#include "cocos2d.h"
#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseMode.hpp"
#include "modes/Modes.hpp"
#include "cards/Cards.hpp"


struct sDay
{
	std::vector<int> data;
};
struct sSem
{
	std::vector<sDay> days;
};
struct sClass
{
	std::vector<sSem> semesters;
};
struct sSchool
{
	std::vector<sClass> classes;
};

class cUserDefault
{
private:
	static cUserDefault *pInstance;

	void loadLevels();
	void loadProfile();
	void createStats();
	void loadCards();
	void loadModes();

	cUserDefault();
public:
	rapidjson::Document levels;
	rapidjson::Document profile;
	rapidjson::Document gameStats;
	rapidjson::Document generatedCards;
	rapidjson::Document generatedModes;

	std::string generateCard(int rare = -1);
	std::string generateCard(std::vector<int> subjects);

	std::vector<sSchool> schools;
	std::vector<cBaseMode*> modes;
	std::vector<cCard*> cards;

	cBaseMode *activeMode;


	cocos2d::TTFConfig *font, *fontHd;

	void saveProfile();

	void saveModes();

	void saveCards();
	static cUserDefault* getInstance();
	~cUserDefault();

	void updateCurrentModes();
};

#endif // !USER_DEFAULT_HPP
