#include "verificationWork.hpp"

cVerificationWork::cVerificationWork(std::string data)
{

}

cVerificationWork::~cVerificationWork()
{
}

cDinamicLayer * cVerificationWork::getNode(int type, cocos2d::Size size)
{
	switch (type)
	{
	case 0: return new cVWIntro(size, this);
		break;
	case 1: return new cVWGame(size, this);
		break;
	case 2: return new cVWOutro(size, this);
		break;
	default:
		break;
	}
	return nullptr;
}


std::string cVerificationWork::generateMode()
{
	std::string result = "1/";

	int MIN_TURNS = 15;
	int MAX_TURNS = 30;
	int STEP_TURNS = 1;
	int MIN_LINE = 3;
	int MAX_LINE = 15;

	cUserDefault *userDefault = cUserDefault::getInstance();
	auto level = userDefault->profile["aLevel"];
	int sh = level[0] - 1;
	int cl = level[1] - 1;
	int sem = level[2] - 1;
	int day = level[3] - 1;
	std::vector<int> groupLevel = { sh , cl ,sem, day };

	///Dinamic skill wrong!!!!!!!!!!!!
	auto skills = userDefault->profile["aStats"];
	std::vector<float> playerSkills;
	float minPS = skills[0], maxPS = skills[0];
	for (unsigned int i = 0; i < skills.size(); i++)
	{
		float val = skills[i];
		if (val < minPS) minPS = val;
		if (val > maxPS) maxPS = val;
		playerSkills.push_back(val);
	}

	//Генерация предметов
	std::vector<int> var = { 1, 2, 3, 4, 5, 6, 7, 8 };
	std::string subjectGen = "[";
	std::vector<int> subjectVec;
	for (int i = 0; i < 4; i++)
	{
		int num = rand() % var.size();
		subjectVec.push_back(var.at(num));
		subjectGen += std::to_string(var.at(num));
		var.erase(var.begin() + num);
		if (i < 3)subjectGen += ",";
	}
	subjectGen += "]";
	result += subjectGen + "|";

	int turns = MIN_TURNS + (rand() % (int((MAX_TURNS - MIN_TURNS) / STEP_TURNS))) * STEP_TURNS;
	result += std::to_string(turns) + "|";

	float minScore = MIN_LINE * turns * minPS;
	float maxScore = MAX_LINE * turns * incr.at(MAX_LINE - 1) * maxPS;
	//maxScore = minScore + ( maxScore - minScore ) / 2.0f;

	//minScore = maxScore / 2.0f;
	maxScore = maxScore * 0.8f; //FOR MY SKILL
	result += std::to_string(int(minScore + (maxScore - minScore) / 6.0f)); //middle


	std::string data = userDefault->generateCard(subjectVec);
	std::vector<std::string> vecStr = Split(data, '/');

	cTeacherCard *card = new cTeacherCard(vecStr.at(0), vecStr.at(1));
	userDefault->cards.push_back(card);

	result += "|" + vecStr.at(0);

	//day cost

	return result;
}
