#ifndef __WelcomeScene_SCENE_H__
#define __WelcomeScene_SCENE_H__

#include "cocos2d.h"


class WelcomeScene : public cocos2d::CCLayer
{
public:
	~WelcomeScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(WelcomeScene);

	virtual void keyBackPressed();

public:
	void createBG();
	void createButtons();
	void onPlay(CCObject *pObject);
	void onMusic(CCObject *pObject);
	void onHelp(CCObject *pObject);
	void onExit(CCObject *pObject);
};

#endif  // __WelcomeScene_SCENE_H__