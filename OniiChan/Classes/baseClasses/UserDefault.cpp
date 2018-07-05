#include "UserDefault.hpp"

cUserDefault *cUserDefault::pInstance = nullptr;

cUserDefault::cUserDefault()
{
	font = new cocos2d::TTFConfig("fonts/chalk.ttf", 50);
	fontHd = new cocos2d::TTFConfig("fonts/chalk.ttf", 100);

	activeMode = nullptr;
	//gameType = -1;
	/*if (!cocos2d::FileUtils::getInstance()->isFileExist("generatedLevels.json")) */loadLevels();
	//else levels.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile("generatedLevels.json").c_str());

	/*if (!cocos2d::FileUtils::getInstance()->isFileExist("profile.json"))*/ loadProfile();
	//else profile.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile("profile.json").c_str());

	loadCards();

	if (!cocos2d::FileUtils::getInstance()->isFileExist("stats.json")) createStats();
	else gameStats.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile("stats.json").c_str());

	//updateCurrentModes();

	/*for(unsigned int i = 0; i < 20; i++)
	{
		std::string data = generateCard(0);		

		auto students = generatedCards.FindMember("studentCards");
		int count = students->value.MemberCount();

		std::string k = "st" + std::to_string(count + 1);
		rapidjson::Value key(k.c_str(), k.size(), generatedCards.GetAllocator());
		rapidjson::Value val(data.c_str(), data.size(), generatedCards.GetAllocator());
		students->value.AddMember(key, val, generatedCards.GetAllocator());
	}
	saveCards();*/
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
	for (auto it : modes)delete it;
	modes.clear();

	auto level = profile.FindMember("aLevel");
	int sh = level->value[0].GetInt() - 1;
	int cl = level->value[1].GetInt() - 1;
	int sem = level->value[2].GetInt() - 1;
	int day = level->value[3].GetInt() - 1;

	std::vector<int> data = schools.at(sh).classes.at(cl).semesters.at(sem).days.at(day).data;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		switch (data.at(i))
		{
			case -6:
			{
				break;
			}
			case -5:
			{
				break;
			}
			case -4:
			{
				cSMLvlUp *lvlUp = new cSMLvlUp("md_" + std::to_string(i));
				modes.push_back(lvlUp);
				break;
			}
			case -3:
			{
				break;
			}
			case -2:
			{
				break;
			}
			case -1:
			{
				cSMCard *card = new cSMCard("md_" + std::to_string(i));
				modes.push_back(card);
				break;
			}
			case 0:
			{
				cMMTurns *turn = new cMMTurns("md_" + std::to_string(i));
				modes.push_back(turn);
				break;
			}
			case 1:
			{
				/*cMMTime *time = new cMMTime();
				modes.push_back(time);
				break;*/
			}
			case 2:
			{
				/*cMMCount *count = new cMMCount();
				modes.push_back(count);
				break;*/
			}
		}
	}
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
	rapidjson::Document nameBase;
	nameBase.Parse(str.c_str());


	int sex = rand() % 100;
	std::string name;
	std::string path;
	if (sex < 30)
	{
		int sz = nameBase.FindMember("mans")->value.Size();

		int num = rand() % sz;
		name = nameBase.FindMember("mans")->value[num].GetString();
		path = "M.png";
	}
	else 
	{
		int sz = nameBase.FindMember("womans")->value.Size();

		int num = rand() % sz;
		name = nameBase.FindMember("womans")->value[num].GetString();
		path = "W.png";
	}
	int subject = rand() % 8;

	auto stats = profile.FindMember("aStats");
	float statVal = stats->value[subject].GetFloat() * statScale;

	std::string result = name + "|"  + path + "|" + std::to_string(subject) + ":" + std::to_string(statVal) + "|" + std::to_string(rareClass);

	auto students = generatedCards.FindMember("studentCards");
	int count = students->value.MemberCount();

	std::string k = "st_" + std::to_string(count + 1);

	rapidjson::Value key(k.c_str(), k.size(), generatedCards.GetAllocator());
	rapidjson::Value val(result.c_str(), result.size(), generatedCards.GetAllocator());
	students->value.AddMember(key, val, generatedCards.GetAllocator());
	saveCards();

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
	rapidjson::Document nameBase;
	nameBase.Parse(str.c_str());


	int sex = rand() % 100;
	std::string name;
	std::string path;
	if (sex < 30)
	{
		int sz = nameBase.FindMember("mans")->value.Size();

		int num = rand() % sz;
		name = nameBase.FindMember("mans")->value[num].GetString();
		path = "MT.png";
	}
	else
	{
		int sz = nameBase.FindMember("womans")->value.Size();

		int num = rand() % sz;
		name = nameBase.FindMember("womans")->value[num].GetString();
		path = "WT.png";
	}
	int subject = subjects.at(rand() % 4);

	std::string result = name + "|" + path + "|" + std::to_string(subject) + ":" + std::to_string(statScale) + "|" + std::to_string(rareClass);


	auto teachers = generatedCards.FindMember("teacherCards");
	int count = teachers->value.MemberCount();

	std::string k = "tr_" + std::to_string(count + 1);
	rapidjson::Value key(k.c_str(), k.size(), generatedCards.GetAllocator());
	rapidjson::Value val(result.c_str(), result.size(), generatedCards.GetAllocator());
	teachers->value.AddMember(key, val, generatedCards.GetAllocator());
	saveCards();

	return k + "/" + result;
}

void cUserDefault::loadProfile()
{
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile("config/profile.json");
	profile.Parse(file.c_str());
}
void cUserDefault::saveProfile()
{
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	profile.Accept(writer);
	cocos2d::FileUtils::getInstance()->writeStringToFile(strbuf.GetString(), "config/profile.json");
}

void cUserDefault::createStats()
{
}

void cUserDefault::loadCards()
{
	auto pt = cocos2d::FileUtils::getInstance()->getWritablePath();
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->getWritablePath() + "cards.json");
	if(!file.empty())generatedCards.Parse(file.c_str());
	else
	{
		generatedCards.Parse("{\"studentCards\":{},\"teacherCards\":{}}");
		saveCards();
	}

	cards.clear();
	auto students = generatedCards.FindMember("studentCards");
	for (rapidjson::Value::MemberIterator itr = students->value.MemberBegin(); itr != students->value.MemberEnd(); ++itr)
	{
		std::string id = itr->name.GetString();
		std::string data = itr->value.GetString();	
		cStudentCard *card = new cStudentCard(id, data);
		cards.push_back(card);		
	}
	auto teachers = generatedCards.FindMember("teacherCards");
	for (rapidjson::Value::MemberIterator itr = teachers->value.MemberBegin(); itr != teachers->value.MemberEnd(); ++itr)
	{
		std::string id = itr->name.GetString();
		std::string data = itr->value.GetString();
		cTeacherCard *card = new cTeacherCard(id, data);
		cards.push_back(card);
	}
}
void cUserDefault::saveCards()
{
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	generatedCards.Accept(writer);
	cocos2d::FileUtils::getInstance()->writeStringToFile(strbuf.GetString(), cocos2d::FileUtils::getInstance()->getWritablePath() + "cards.json");
	loadCards();
}

void cUserDefault::saveModes()
{
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	generatedModes.Accept(writer);
	cocos2d::FileUtils::getInstance()->writeStringToFile(strbuf.GetString(), cocos2d::FileUtils::getInstance()->getWritablePath() + "modes.json");
}
void cUserDefault::loadModes()
{
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->getWritablePath() + "modes.json");
	if (!file.empty())generatedModes.Parse(file.c_str());
	else
	{
		generatedModes.Parse("{}");
		saveModes();
	}
}

void cUserDefault::loadLevels()
{
	std::string file = cocos2d::FileUtils::getInstance()->getStringFromFile("config/levels.json");
	levels.Parse(file.c_str());
	for (rapidjson::Value::MemberIterator itr = levels.MemberBegin(); itr != levels.MemberEnd(); ++itr)
	{
		std::string name = itr->name.GetString();
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

		for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++)
		{
			sDay day;
			auto dt = itr->value[i].GetArray();

			for (unsigned int j = 0; j < dt.Size(); j++)
			{
				day.data.push_back(dt[j].GetInt());
			}

			schools.at(lvlNum.at(0) - 1).classes.at(lvlNum.at(1) - 1).semesters.at(lvlNum.at(2) - 1).days.push_back(day);
		}
	}

	loadModes();
}
