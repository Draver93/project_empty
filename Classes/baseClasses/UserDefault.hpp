#ifndef USER_DEFAULT_HPP
#define USER_DEFAULT_HPP

#include "json.hpp"
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
	nlohmann::json levels;
	nlohmann::json profile;
	nlohmann::json gameStats;
	nlohmann::json generatedCards;
	nlohmann::json generatedModes;

	std::string generateCard(int rare = -1);
	std::string generateCard(std::vector<int> subjects);

	std::vector<sSchool> schools;
	std::vector<cBaseMode*> modes;
	std::vector<cCard*> cards;

	cBaseMode *activeMode;
	cocos2d::TTFConfig *font, *fontHd;

	bool first_start;

	void saveProfile();
	void saveModes();
	void saveCards();

	static cUserDefault* getInstance();
	~cUserDefault();

	void updateCurrentModes();
};

#endif // !USER_DEFAULT_HPP
