#include "UserDefault.hpp"

cUserDefault *cUserDefault::pInstance = nullptr;

cUserDefault::cUserDefault()
{
	first_start = false;
	nlohmann::json wow;
	std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile("config/profile.json").c_str();
	wow = nlohmann::json::parse(data);
	font = new cocos2d::TTFConfig("fonts/chalk.ttf", 50);
	fontHd = new cocos2d::TTFConfig("fonts/chalk.ttf", 100);

	activeMode = nullptr;
	loadLevels();
	loadProfile();
	loadCards();
	loadHistory();

	if (!cocos2d::FileUtils::getInstance()->isFileExist("stats.json")) createStats();
	else gameStats = nlohmann::json::parse(cocos2d::FileUtils::getInstance()->getStringFromFile("stats.json").c_str());
}
cUserDefault::~cUserDefault()
{

}
cUserDefault * cUserDefault::getInstance()
{
	if (pInstance != nullptr)return pInstance;
	else
	{
		pInstance = new cUserDefault();
		return pInstance;
	}

	return nullptr;
}

void cUserDefault::updateCurrentModes()
{
	generatedModes.size();
	for (auto it : modes)delete it;
	modes.clear();
	auto level = profile["aLevel"];

	int size = generatedModes["data"].size();
	if (size < 8)
		for (int i = 0; i < 8 - size; i++)
		{
			std::string data;
			//getNextLevelType
			int next = 1;// rand() % 6;
		
			switch (next)
			{
				case 0: data = cTestWork::generateMode();
					break;
				case 1: data = cVerificationWork::generateMode();
					break;
				case 2: data = cIndependentWork::generateMode();
					break;
				case 3: data = cCourseWork::generateMode();
					break;
				case 4: data = cRest::generateMode();
					break;
				case 5: data = cSchoolCircle::generateMode();
					break;
				case 6: data = cTraining::generateMode();
					break;
			}
			generatedModes["data"].push_back(data);
		}

	for (nlohmann::json::iterator it = generatedModes["data"].begin(); it != generatedModes["data"].end(); ++it) 
	{
		std::string data = it.value();
		std::vector<std::string> vec = Split(data, '/');

		switch (std::stoi(vec.at(0)))
		{
			case 0:
			{
				cTestWork *mode = new cTestWork(vec.at(1));
				modes.push_back(mode);
				break;
			}
			case 1:
			{
				cVerificationWork *mode = new cVerificationWork(vec.at(1));
				modes.push_back(mode);
				break;
			}
			case 2:
			{
				cIndependentWork *mode = new cIndependentWork(vec.at(1));
				modes.push_back(mode);
				break;
			}
			case 3:
			{
				cCourseWork *mode = new cCourseWork(vec.at(1));
				modes.push_back(mode);
				break;
			}
			case 4:
			{
				cRest *mode = new cRest(vec.at(1));
				modes.push_back(mode);
				break;
			}
			case 5:
			{
				cSchoolCircle *mode = new cSchoolCircle(vec.at(1));
				modes.push_back(mode);
				break;
			}
			case 6:
			{
				cTraining *mode = new cTraining(vec.at(1));
				modes.push_back(mode);
				break;
			}
		}
	}
	saveModes();
	/*int sh = level.at(0) - 1;
	int cl = level.at(1) - 1;
	int sem = level.at(2) - 1;
	int day = level.at(3) - 1;

	std::vector<int> data = schools.at(sh).classes.at(cl).semesters.at(sem).days.at(day).data;*/

}


std::string cUserDefault::generateCard(int rare)
{
	std::vector<float> RARE = { 50.0f, 75.0f, 89.5f, 100.0f };
	std::vector<float> RARE_FACTOR = { 1.0f, 2.0f, 4.0f, 8.0f };

	float rareVal = (rand() % 1000) / 10.0f;
	if (rare != -1) rareVal = RARE.at(rare);

	int rareClass = -1;
	for(unsigned int i = 0; i < RARE.size(); i++) 
	{
		if (RARE.at(i) >= rareVal) 
		{
			rareClass = i; 
			break; 
		}
	}
	if (rareClass == -1)rareClass = 3;

	float rareMinF = 0.5f;
	if(rareClass - 1 >= 0) rareMinF = RARE_FACTOR.at(rareClass - 1);
	float rareMaxF = RARE_FACTOR.at(rareClass);

	int randFactorScale = rand() % 100;
	float statScale = rareMinF + (rareMaxF - rareMinF) / 100.0f * randFactorScale;

	//База имен
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config/names.json");
	nlohmann::json nameBase = nlohmann::json::parse(str);


	int sex = rand() % 100;
	std::string name;
	std::string path;
	if (sex < 30)
	{
		int sz = nameBase["mans"].size();

		int num = rand() % sz;
		name = nameBase["mans"].at(num).get<std::string>();
		path = "M.png";
	}
	else 
	{
		int sz = nameBase["womans"].size();

		int num = rand() % sz;
		name = nameBase["womans"].at(num).get<std::string>();
		path = "W.png";
	}
	int subject = rand() % 8;

	auto stats = profile["aStats"];
	float statVal = stats.at(subject).get<float>();

	std::string result = name + "|"  + path + "|" + std::to_string(subject) + ":" + std::to_string(statVal) + "|" + std::to_string(rareClass);

	auto &students = generatedCards["studentCards"];
	int count = students.size();

	std::string k = "st_" + std::to_string(count + 1);
	students[k] = result;

	return k + "/" + result;
}
std::string cUserDefault::generateCard(std::vector<int> subjects)
{
	std::vector<float> RARE = { 50.0f, 75.0f, 89.5f, 100.0f };
	std::vector<float> RARE_FACTOR = { 1.0f, 0.5f, 0.3f, 0.1f };

	float rareVal = (rand() % 1000) / 10.0f;
	int rareClass = 0;	
	for (unsigned int i = 0; i < RARE.size(); i++)
	{
		if (RARE.at(i) > rareVal)
		{
			rareClass = i;
			break;
		}
	}

	float rareMinF = 0.5f;
	if (rareClass - 1 >= 0) rareMinF = RARE_FACTOR.at(rareClass - 1);
	float rareMaxF = RARE_FACTOR.at(rareClass);

	int randFactorScale = rand() % 100;
	float statScale = rareMinF + (rareMaxF - rareMinF) / 100.0f * randFactorScale;

	//База имен
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config/names.json");
	nlohmann::json nameBase;
	nameBase = nlohmann::json::parse(str.c_str());


	int sex = rand() % 100;
	std::string name;
	std::string path;
	if (sex < 30)
	{
		int sz = nameBase["mans"].size();

		int num = rand() % sz;
		name = nameBase["mans"][num].get<std::string>();
		path = "MT.png";
	}
	else
	{
		int sz = nameBase["womans"].size();

		int num = rand() % sz;
		name = nameBase["womans"][num].get<std::string>();
		path = "WT.png";
	}
	int subject = subjects.at(rand() % 4);

	auto &teachers = generatedCards["teacherCards"];
	int count = teachers.size();

	std::string result = name + "|" + path + "|" + std::to_string(subject) + ":" + std::to_string(statScale) + "|" + std::to_string(rareClass);
	std::string k = "tr_" + std::to_string(count + 1);


	teachers[k] = result;

	saveCards();

	return k + "/" + result;
}

void cUserDefault::loadProfile()
{
	auto pt = cocos2d::FileUtils::getInstance()->getWritablePath();
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->getWritablePath() + "profile.json");
	if (!file.empty())profile = nlohmann::json::parse(file);
	else
	{
		profile = nlohmann::json::parse("{\"sName\": \"\", \"sAvatarName\" : \"\", \"bSound\" : false,\"bSex\" : true,\"bTutorial\" : false,\"fAverageScore\" : 0.0,\"fBestScore\" : 0.0,\"fLastScore\" : 0.0,\"aLevel\" : [1, 1, 1, 1], \"aStats\": [0.2,0.5,1.3,0.5,0.1,0.2,0.7,0.3], \"sDifficulty\": \"[1,1]|[1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0]\", \"aOpenCards\" : [], \"aActiveCards\" : []}");
		saveProfile();
		first_start = true;
	}

}
void cUserDefault::saveProfile()
{
	cocos2d::FileUtils::getInstance()->writeStringToFile(profile.dump(), cocos2d::FileUtils::getInstance()->getWritablePath() + "profile.json");
}

void cUserDefault::createStats()
{
}

void cUserDefault::loadCards()
{
	auto pt = cocos2d::FileUtils::getInstance()->getWritablePath();
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->getWritablePath() + "cards.json");
	if(!file.empty())generatedCards = nlohmann::json::parse(file);
	else
	{
		generatedCards = nlohmann::json::parse("{\"studentCards\":{},\"teacherCards\":{}}");
		saveCards();
	}

	cards.clear();
	auto students = generatedCards["studentCards"];
	for (auto it = students.begin(); it != students.end(); ++it)
	{
		std::string id = it.key();
		std::string data = it.value();
		cStudentCard *card = new cStudentCard(id, data);
		cards.push_back(card);		
	}
	auto teachers = generatedCards["teacherCards"];
	for (auto it = teachers.begin(); it != teachers.end(); ++it)
	{
		std::string id = it.key();
		std::string data = it.value();
		cTeacherCard *card = new cTeacherCard(id, data);
		cards.push_back(card);
	}
}
void cUserDefault::saveCards()
{
	cocos2d::FileUtils::getInstance()->writeStringToFile(generatedCards.dump(), cocos2d::FileUtils::getInstance()->getWritablePath() + "cards.json");
	loadCards();
}


void cUserDefault::saveModes()
{
	cocos2d::FileUtils::getInstance()->writeStringToFile(generatedModes.dump(), cocos2d::FileUtils::getInstance()->getWritablePath() + "modes.json");
}
void cUserDefault::loadModes()
{
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->getWritablePath() + "modes.json");
	if (!file.empty())generatedModes = nlohmann::json::parse(file.c_str());
	else
	{
		generatedModes = nlohmann::json::parse("{}");
		saveModes();
	}
}

void cUserDefault::addHistoryElement(std::string key, std::string name, int result)
{
	//history.erase("one");
	history[key] = name + "|" + std::to_string(result);
}
void cUserDefault::saveHistory()
{
	cocos2d::FileUtils::getInstance()->writeStringToFile(history.dump(), cocos2d::FileUtils::getInstance()->getWritablePath() + "history.json");
	loadCards();
}
void cUserDefault::loadHistory()
{
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->getWritablePath() + "history.json");
	if (!file.empty())history = nlohmann::json::parse(file.c_str());
	else
	{
		history = nlohmann::json::parse("{}");
		saveHistory();
	}
}



void cUserDefault::loadLevels()
{
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile("config/levels.json");
	levels = nlohmann::json::parse(file);
	for (auto it_level = levels.begin(); it_level != levels.end(); ++it_level)
	{
		std::string name = it_level.key();
		std::vector<std::string> data = Split(name, '|');
		std::vector<int> lvlNum;
		for (auto it : data)lvlNum.push_back(std::stoi(it));
		if ((int)schools.size() < lvlNum.at(0))
		{
			sSchool sc; schools.push_back(sc);
		}
		if (int(schools.at(lvlNum.at(0) - 1).classes.size()) < lvlNum.at(1))
		{
			sClass cl; schools.at(lvlNum.at(0) - 1).classes.push_back(cl);
		}
		if (int(schools.at(lvlNum.at(0) - 1).classes.at(lvlNum.at(1) - 1).semesters.size()) < lvlNum.at(2))
		{
			sSem sem; schools.at(lvlNum.at(0) - 1).classes.at(lvlNum.at(1) - 1).semesters.push_back(sem);
		}

		for (int i = 0; i < it_level.value().size(); i++)
		{
			sDay day;
			auto dt = it_level.value().at(i);

			for (unsigned int j = 0; j < dt.size(); j++)
			{
				day.data.push_back(dt[j]);
			}

			schools.at(lvlNum.at(0) - 1).classes.at(lvlNum.at(1) - 1).semesters.at(lvlNum.at(2) - 1).days.push_back(day);
		}
	}

	loadModes();
}
