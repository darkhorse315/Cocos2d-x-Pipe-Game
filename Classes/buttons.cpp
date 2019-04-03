#include "buttons.h"
#include "GameScene.h"
#include "GameData.h"
#include "LoadingScene.h"
#include "GameSound.h"
using namespace cocos2d;

Buttons* Buttons::createWithParent(CCNode *parent)
{
	Buttons* object = new Buttons;
	object->initWithParent(parent);
	object->autorelease();
	return object;
}
void Buttons::initWithParent(CCNode* parent)
{
	CCMenu* menu = CCMenu::create();
	parent->addChild(menu);

	CCMenuItemToggle *musicButton = CCMenuItemToggle::createWithTarget(this, menu_selector(Buttons::onMusic), 
		CCMenuItemImage::create("buttons/sound_on_btn.png", "buttons/sound_on_btn_press.png"), 
		CCMenuItemImage::create("buttons/sound_off_btn.png", "buttons/sound_off_btn_press.png"), NULL);
	
	m_stateButton = CCMenuItemToggle::createWithTarget(this, menu_selector(Buttons::onStateButton), 
		CCMenuItemImage::create("buttons/back_btn.png", "buttons/back_btn_press.png", "buttons/back_btn_disabled.png"), 
		CCMenuItemImage::create("buttons/continue_btn.png", "buttons/continue_btn_press.png", "buttons/continue_btn_disabled.png"), NULL);
	m_repairButton = CCMenuItemImage::create("buttons/repair_btn.png", "buttons/repair_btn_press.png", "buttons/repair_btn_disabled.png",
		this, menu_selector(Buttons::onRepair));
	
	menu->setPosition(ccp(0, 0));
	menu->addChild(musicButton);
	menu->addChild(m_stateButton);
	menu->addChild(m_repairButton);
	
	m_stateButton->setPosition(getPos(1035.8, 724 - 41.2));
	m_repairButton->setPosition(getPos(905.8, 724 - 41.2));
	musicButton->setPosition(getPos(784.7, 724 - 41.2));

	musicButton->setSelectedIndex(GameData::sharedGameData()->isSoundTurnedOn ? 0 : 1);

	//repair count label
	m_repairLabel = CCLabelTTF::create("3", "Arial", 25);
	CCSize size = m_repairButton->getContentSize();
	m_repairLabel->setPosition(getPos(size.width * 0.5 + 50, size.height * 0.5 + 10));
	m_repairButton->addChild(m_repairLabel);
}
void Buttons::reset()
{
	m_mode = modeBreak;
	m_repairCount = 3;

	setStateEnabled(false);
	setRepairEnabled(false);
}

void Buttons::onMusic(CCObject *pObject)
{
	GameSound::sharedGameSound()->playEffect(BUTTON_CLICKED);
	GameSound::sharedGameSound()->switchSound();
}
void Buttons::onStateButton(CCObject *pObject)
{
	if(m_mode == modeNext)
		onNext();
	else
		onBreak();

	GameSound::sharedGameSound()->stopAllEffects();
	GameSound::sharedGameSound()->playEffect(BUTTON_CLICKED);
}
void Buttons::onRepair(CCObject *pObject)
{
	if(m_repairCount <= 1)
		m_repairButton->setEnabled(false);

	m_repairCount --;
	m_repairLabel->setString(CCString::createWithFormat("%d", m_repairCount)->getCString());
	setRepairEnabled(false);

	GameScene::sharedGameScene()->setRepairMode();

	GameSound::sharedGameSound()->playEffect(BUTTON_CLICKED);
}
void Buttons::onBreak()
{
	CCScene* scene = LoadingScene::createWithTargetScene(TargetSceneWelcomeScene);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void Buttons::onNext()
{
	m_mode = modeBreak;
	GameScene::sharedGameScene()->nextLevel();
}
void Buttons::setStateEnabled(bool flag)
{
	m_stateButton->setEnabled(flag);
}
void Buttons::setRepairEnabled(bool flag)
{
	m_repairButton->setEnabled(flag);
	if(flag)
		m_repairLabel->setOpacity(255);
	else
		m_repairLabel->setOpacity(150);
}
void Buttons::setModeNext()
{
	m_mode = modeNext;
	m_stateButton->setSelectedIndex(1);

	m_repairCount = 3;
	m_repairLabel->setString("3");
}
Buttons::~Buttons()
{

}