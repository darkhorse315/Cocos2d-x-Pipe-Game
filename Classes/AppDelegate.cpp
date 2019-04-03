#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "LoadingScene.h"
#include "SimpleAudioEngine.h"
#include "GameSound.h"
#include "GameData.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    CCDirector *pDirector = CCDirector::sharedDirector();
	
	pDirector->setOpenGLView(eglView);
	eglView->setFrameSize(800, 480);

	eglView->setDesignResolutionSize(800, 480, kResolutionExactFit);

    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);


    // create a scene. it's an autorelease object
	CCScene *pScene = LoadingScene::createWithTargetScene(TargetSceneWelcomeScene);

    // run
    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
	GameSound::sharedGameSound()->resumeBackgroundMusic();
}
