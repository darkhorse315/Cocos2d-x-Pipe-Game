#include "WelcomeScene.h"
#include "LoadingScene.h"
#include "GameSound.h"
#include "GameData.h"
using namespace cocos2d;

WelcomeScene::~WelcomeScene()
{
	CCTextureCache::sharedTextureCache()->removeAllTextures();
}

CCScene* WelcomeScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        WelcomeScene *layer = WelcomeScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool WelcomeScene::init()
{
	if(!CCLayer::init())
		return false;

	GameSound::sharedGameSound()->playBackgroundMusic(WELCOME_MUSIC);

	createBG();
	createButtons();
	
	return true;
}

void WelcomeScene::createBG()
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *bg = CCSprite::create("back.png");
	addChild(bg);
	bg->setAnchorPoint(ccp(0, 0));

	CCSprite *title = CCSprite::create("title.png");
	addChild(title);
	title->setPosition(getPos(1152 * 0.5, 430));
}

void WelcomeScene::createButtons()
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemImage *	playButton  = CCMenuItemImage::create("buttons/start_btn.png", "buttons/start_btn_press.png", this, menu_selector(WelcomeScene::onPlay));
	CCMenuItemImage *	exitButton  = CCMenuItemImage::create("buttons/end_btn.png", "buttons/end_btn_press.png", this, menu_selector(WelcomeScene::onExit));
	CCMenuItemImage *	helpButton  = CCMenuItemImage::create("buttons/help_btn.png", "buttons/help_btn_press.png", this, menu_selector(WelcomeScene::onHelp));
	CCMenuItemToggle *	musicButton = CCMenuItemToggle::createWithTarget(this, menu_selector(WelcomeScene::onMusic), 
																		 CCMenuItemImage::create("buttons/sound_on_btn.png", "buttons/sound_on_btn_press.png"), 
																		 CCMenuItemImage::create("buttons/sound_off_btn.png", "buttons/sound_off_btn_press.png"), NULL);

	CCMenu *menu = CCMenu::create();
	menu->setPosition(0, 0);
	addChild(menu);
	menu->addChild(playButton);
	menu->addChild(helpButton);
	menu->addChild(musicButton);
	menu->addChild(exitButton);

	playButton->setPosition(getPos(730.1f, 724 - 482.5f));
	exitButton->setPosition(getPos(730.1f, 724 - 609.5f));
	helpButton->setPosition(getPos(1030, 724 - 50));
	musicButton->setPosition(getPos(1100, 724 - 50));

	musicButton->setSelectedIndex(GameData::sharedGameData()->isSoundTurnedOn ? 0 : 1);
}

void WelcomeScene::onPlay(CCObject *pObject)
{
	CCScene* scene = LoadingScene::createWithTargetScene(TargetSceneGameScene);
	CCDirector::sharedDirector()->replaceScene(scene);

	GameSound::sharedGameSound()->playEffect(BUTTON_CLICKED);
}

void WelcomeScene::onMusic(CCObject *pObject)
{
	GameSound::sharedGameSound()->switchSound();
	GameSound::sharedGameSound()->playEffect(BUTTON_CLICKED);
}

void WelcomeScene::onHelp(CCObject *pObject)
{
	CCScene* scene = LoadingScene::createWithTargetScene(TargetSceneHelpScene);
	CCDirector::sharedDirector()->replaceScene(scene);

	GameSound::sharedGameSound()->playEffect(BUTTON_CLICKED);
}
void WelcomeScene::onExit(CCObject *pObject)
{
	CCDirector::sharedDirector()->end();
}

