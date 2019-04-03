#pragma once
#include "cocos2d.h"

enum{
	modeNext,
	modeBreak,
};

class Buttons : public cocos2d::CCObject
{
public:
	~Buttons(void);
	static Buttons* createWithParent(cocos2d::CCNode *parent);
	void initWithParent(cocos2d::CCNode* parent);
	void reset();

	void onMusic(cocos2d::CCObject *pObject);
	void onStateButton(cocos2d::CCObject *pObject);
	void onRepair(cocos2d::CCObject *pObject);

	void setStateEnabled(bool flag);
	void setRepairEnabled(bool flag);
	void onBreak();
	void onNext();
	void setModeNext();
private:
	cocos2d::CCMenuItemToggle *m_stateButton;
	cocos2d::CCMenuItemImage *m_repairButton;
	cocos2d::CCLabelTTF* m_repairLabel;
	int m_mode;
public:
	int m_repairCount;
};
