#ifndef BASE_SCENE_HPP
#define BASE_SCENE_HPP

#include "cocos2d.h"
#include "string"
#include "vector"


enum class SceneState
{
	CLOSE,
	ACTIVE
};


class cBaseScene
{
private:
	//cocos2d::Scene *pScene;
	SceneState state;	
public:
	std::string name;

	cBaseScene();
	~cBaseScene();
	SceneState getState() { return state; };
	virtual cocos2d::Scene *bs_create() = 0;
	virtual void update(float dt) = 0;
};

#endif //BASE_SCENE_HPP