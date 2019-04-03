#include "LoadingScene.h"
#include "WelcomeScene.h"
#include "GameScene.h"
#include "GameSound.h"
#include "GameData.h"

using namespace cocos2d;
LoadingScene* LoadingScene::createWithTargetScene(TargetScenes targetScene)
{
	LoadingScene* scene = new LoadingScene;
	scene->autorelease();
	if(scene->initWithTargetScene(targetScene))
		return scene;
	else
		return NULL;
}
bool LoadingScene::initWithTargetScene(TargetScenes targetScene)
{
	if(!CCScene::init())
		return false;

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite *bg = CCSprite::create("back.png");
	addChild(bg);
	bg->setAnchorPoint(ccp(0, 0));

	CCSprite *waitSpr = CCSprite::create("letters/loading.png");
	addChild(waitSpr);
	waitSpr->setPosition(getPos(1152 * 0.5 + 100, 724 * 0.5));

	m_targetScene = targetScene;

	//must wait one frame before loading the target scene
	scheduleUpdate();
	return true;
}
void LoadingScene::update(float dt)
{
	unscheduleAllSelectors();

	switch(m_targetScene)
	{
	case TargetSceneWelcomeScene:
		GameSound::create();
		GameData::create();
		CCDirector::sharedDirector()->replaceScene(WelcomeScene::scene());
		break;
	case TargetSceneGameScene:
		CCDirector::sharedDirector()->replaceScene(GameScene::scene());
		break;
	}
	if(m_targetScene == TargetSceneHelpScene)
	{
		//---------help scene-----------
		CCScene* help_scene = CCScene::create();
		CCSprite* help = CCSprite::create("help.png");
		help->setAnchorPoint(ccp(0, 0));
		help_scene->addChild(help);

		CCMenuItemImage* back = CCMenuItemImage::create("buttons/back_btn.png", "buttons/back_btn_press.png", this, menu_selector(LoadingScene::onBack));
		CCMenu* menu = CCMenu::create(back, NULL);
		help_scene->addChild(menu);
		back->setPosition(getPos(400, 315));

		CCDirector::sharedDirector()->replaceScene(help_scene);
	}
}
void LoadingScene::onBack(CCObject* pObject)
{
	CCScene* scene = LoadingScene::createWithTargetScene(TargetSceneWelcomeScene);
	CCDirector::sharedDirector()->replaceScene(scene);
}

