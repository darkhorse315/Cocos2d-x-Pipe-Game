#pragma once
#include "cocos2d.h"

typedef enum{
	TargetSceneInvalid,
	TargetSceneWelcomeScene,
	TargetSceneGameScene,
	TargetSceneHelpScene,
	TargetSceneMax
} TargetScenes;
class LoadingScene : public cocos2d::CCScene
{
public:
	static LoadingScene* createWithTargetScene(TargetScenes targetScene);
	bool initWithTargetScene(TargetScenes targetScene);
	virtual void update(float dt);
	void onBack(cocos2d::CCObject* pObject);
private:
	TargetScenes m_targetScene;
};
