#include "GameScene.h"
#include "GameData.h"
#include "Pipe.h"
#include "GameSound.h"
#include "LoadingScene.h"

using namespace cocos2d;

GameScene *globalGameScene = NULL;
GameScene::~GameScene(void)
{
	globalGameScene = NULL;

	m_mainpad->destroy();
	m_buttons->release();

	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeAllTextures();
}
GameScene* GameScene::sharedGameScene()
{
	if(!globalGameScene)
		globalGameScene = GameScene::create();
	return globalGameScene;
}
CCScene* GameScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GameScene *layer = GameScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool GameScene::init()
{
	if(!CCLayer::init())
		return false;
	
	setKeypadEnabled(true);

	globalGameScene = this;
	srand((unsigned)time(NULL));
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pipe.plist");

	createBG();
	createMainpad();
	createPipeContainer();
	createButtons();
	createCounters();//create time counter, scores, level and so on
	initVariables();

	resetCounters();//reset value of the counters according to member variables
	return true;
}
void GameScene::createBG()
{
	CCSprite* backSpr = CCSprite::create("main_back.png");
	backSpr->setAnchorPoint(ccp(0, 0));
	addChild(backSpr, -1);
}
void GameScene::createMainpad()
{
	m_mainpad = Mainpad::createWithParent(this);
	m_mainpad->setPosition(getPos(626, 724 - 392));
}
void GameScene::createPipeContainer()
{
	m_pipeBatchNode = CCSpriteBatchNode::create("pipe.png");
	addChild(m_pipeBatchNode);
}
void GameScene::createButtons()
{
	m_buttons = new Buttons;
	m_buttons->initWithParent(this);
}
void GameScene::createCounters()
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	//-----------------------create time counter--------------
	m_timeCounter = CCSprite::create("timepad.png");
	addChild(m_timeCounter);
	m_timeCounter->setAnchorPoint(CCPointZero);
	m_timeCounter->setPosition(getPos(87, 724 - 672 + 3));

	CCSprite* timeCounterBack = CCSprite::create("timepad_back.png");
	addChild(timeCounterBack);
	timeCounterBack->setAnchorPoint(CCPointZero);
	timeCounterBack->setPosition(getPos(87, 724 - 672 + 3));

	//----------------------create labels--------------------
	CCLabelBMFont *scoreLabel = CCLabelBMFont::create("", "font.fnt");
	addChild(scoreLabel, 0, tagScoreLabel);
	scoreLabel->setScale(0.7f);
	scoreLabel->setAnchorPoint(CCPointZero);
	scoreLabel->setPosition(getPos(421, 724 - 60));

	CCLabelBMFont *levelLabel = CCLabelBMFont::create("", "font.fnt");
	addChild(levelLabel, 0, tagLevelLabel);
	levelLabel->setScale(0.7f);
	levelLabel->setAnchorPoint(CCPointZero);
	levelLabel->setPosition(getPos(574, 724 - 60));

	CCLabelBMFont *goalLabel = CCLabelBMFont::create("", "font.fnt");
	addChild(goalLabel, 0, tagGoalLabel);
	goalLabel->setScale(0.7f);
	goalLabel->setAnchorPoint(CCPointZero);
	goalLabel->setPosition(getPos(670, 724 - 60));

	CCLabelBMFont *maxScoreLabel = CCLabelBMFont::create("", "font.fnt");
	addChild(maxScoreLabel, 0, tagMaxScoreLabel);
	maxScoreLabel->setScale(0.7f);
	maxScoreLabel->setAnchorPoint(CCPointZero);
	maxScoreLabel->setPosition(getPos(276, 724 - 60));
	
	m_maxScore = GameData::sharedGameData()->maxScore;
	resetMaxScore();
}
void GameScene::initVariables()
{
	//----------------------init counter variables
	m_score = 0;
	m_level = 0;
	m_pipeTotalCount = 20;
	m_time = 80;
	m_showingPipe = NULL;
	m_cntCrashed  = 0;
}
void GameScene::resetCounters()
{
	GameSound::sharedGameSound()->playBackgroundMusic(WELCOME_MUSIC);

	m_pipeConnectedCount = 0;

	//init time counter
	m_timeCounter->setScaleY(0);
	m_timeCounter->setColor(ccc3(255, 255, 255));

	m_buttons->reset();
	m_mainpad->reset();

	//init labels
	addScore(0);
	addLevel(1);
	addGoal(0);
	
	
	unschedule(schedule_selector(GameScene::onPrevStart));
	schedule(schedule_selector(GameScene::onPrevStart), 0.01f);
	
}
void GameScene::addScore(int plusScore)
{
	//-------------plus score by plusScore variable
	m_score += plusScore;
	CCLabelBMFont *scoreLabel = (CCLabelBMFont*)getChildByTag(tagScoreLabel);
	scoreLabel->setString(CCString::createWithFormat("%05d", m_score)->getCString());
}
void GameScene::addLevel(int plusLevel)
{
	//--------------------plus level by plusLevel variable
	m_level += plusLevel;
	CCLabelBMFont *levelLabel = (CCLabelBMFont*)getChildByTag(tagLevelLabel);
	CCString* string  = CCString::createWithFormat("%d", m_level);
	levelLabel->setString(string->getCString());

	//add target goal every 2 levels
	if(m_level % 2 == 1 && m_level > 1)
		m_pipeTotalCount += 5;
	//last level
	if(m_level == 9)
	{
		m_time = 30;
		m_pipeTotalCount = 16;
	}
}
void GameScene::addGoal(int plusGoal)
{
	//-------------------plus goal especially used while the checking of the game
	m_pipeConnectedCount += plusGoal;
	int leftPipeCount = m_pipeTotalCount - m_pipeConnectedCount;
	if(leftPipeCount < 0) leftPipeCount = 0;

	CCLabelBMFont *goalLabel = (CCLabelBMFont*)getChildByTag(tagGoalLabel);
	goalLabel->setString(CCString::createWithFormat("%d", leftPipeCount)->getCString());
}
void GameScene::resetMaxScore()
{
	CCLabelBMFont *maxScoreLabel = (CCLabelBMFont*)getChildByTag(tagMaxScoreLabel);
	maxScoreLabel->setString(CCString::createWithFormat("%05d", m_maxScore)->getCString());
}
void GameScene::showNodeAction(CCSprite* node, CCPoint position, int delay, int zorder)
{
	//-------------------show the string for a while----------------
	addChild(node, zorder);
	node->setPosition(position);
	node->setOpacity(0);
	CCActionInterval* fadeinAction = CCFadeIn::create(1 * delay);
	CCActionInterval* scaleAction = CCScaleBy::create(0.5f * delay, 3);
	CCActionInterval* fadeoutAction = CCFadeOut::create(0.5f * delay);
	CCActionInterval* moveupAction = CCMoveBy::create(0.5f * delay, ccp(0, 50));
	CCCallFuncN *callFunc = CCCallFuncN::create(this, callfuncN_selector(GameScene::onShowNodeFinished));
	
	node->runAction(CCSequence::create(fadeinAction, scaleAction, callFunc, NULL));
	node->runAction(CCSequence::create(fadeinAction, fadeoutAction, NULL));
	node->runAction(CCSequence::create(fadeinAction, moveupAction, NULL));
}
void GameScene::onShowNodeFinished(CCNode *node)
{
	node->removeFromParentAndCleanup(true);
}

void GameScene::onPrevStart(float dt)
{

	float scale = m_timeCounter->getScaleY();
	scale += 0.01f;

	//when it powers up to the top, start the game
	if(scale >= 1)
	{
		unschedule(schedule_selector(GameScene::onPrevStart));
		startTheGame();
		scale = 1;
	}
	m_timeCounter->setScaleY(scale);
}
void GameScene::onStart(float dt)
{
	float scale = m_timeCounter->getScaleY();
	scale -= 0.1 / m_time;

	//if it arrives at the bottom, we'll check the pipes
	if(0.25 <= scale  && scale <= 0.5)
		m_timeCounter->setColor(ccc3(255, 255, 0));
	else if(0 < scale && scale < 0.25)
		m_timeCounter->setColor(ccc3(255, 0, 0));
	else if(scale <= 0)
	{
		unschedule(schedule_selector(GameScene::onStart));
		startCheckingTheGame();
		scale = 0;
	}
	m_timeCounter->setScaleY(scale);
}
void GameScene::startTheGame()
{
	produceRandomPipe();

	m_mainpad->start();
	m_buttons->setStateEnabled(true);

	//------------show the level for a while
	if(m_level < 9)
	{
		/*CCSprite* spr = CCSprite::create("letters/nextlevel.png");
		CCLabelTTF* label = CCLabelTTF::create(CCString::createWithFormat("%d", m_level)->getCString(), "Arial", 30);
		label->setPosition(ccp(-15, spr->getContentSize().height * 0.5));
		spr->addChild(label);
		showNodeAction(spr, m_mainpad->getPosition());*/
	}
	else if(m_level == 9)
	{
		CCSprite* spr = CCSprite::create("letters/endlevel.png");
		showNodeAction(spr, m_mainpad->getPosition());
	}

	schedule(schedule_selector(GameScene::onStart), 0.1f);
}
void GameScene::startCheckingTheGame()
{
	m_mainpad->setTouchEnabled(false);
	m_mainpad->go();
	m_timeCounter->setScaleY(0);
	m_buttons->setRepairEnabled(false);
}
void GameScene::showCurrentPipeFromCategory(categories categoryNumber)
{
	//if it already created, then remove pipe and add new pipe
	if(!m_showingPipe)
	{
		m_showingPipe = new Pipe;
		m_showingPipe->initWithParentAndCategory(m_pipeBatchNode, categoryNumber);
	}
	else
		m_showingPipe->resetPipe(m_pipeBatchNode, categoryNumber);

	m_showingPipe->setPosition(getPos(99, 724 - 54));
	m_mainpad->setPipeType(categoryNumber);
}
void GameScene::produceRandomPipe()
{
	showCurrentPipeFromCategory(Pipe::getRandomCategory());
}
CCSpriteBatchNode* GameScene::getPipeContainer()
{
	return m_pipeBatchNode;
}
void GameScene::createFireworks(int cnt)
{
	schedule(schedule_selector(GameScene::onCreateFirework), 0.5f, cnt - 1, 0);
}
void GameScene::onCreateFirework(float dt)
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	CCParticleSystem* emitter = CCParticleExplosion::create();
	emitter->setLife(2);
	emitter->setLifeVar(2);
	addChild(emitter, 10);
	emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("stars.png") );
	emitter->setAutoRemoveOnFinish(true);

	float randomX = CCRANDOM_0_1() * screenSize.width * 0.3 + screenSize.width * 0.4;
	float randomY = CCRANDOM_0_1() * screenSize.height * 0.3 + screenSize.height * 0.4;
	emitter->setPosition(randomX, randomY);
}
void GameScene::onAllowNext(float dt)
{
	m_buttons->setStateEnabled(true);
}
void GameScene::checkLevel()
{
	if(m_pipeConnectedCount >= m_pipeTotalCount)
	{
		//----------------the finished sprite of the level
		if(m_level < 9)
		{
			CCSprite *success = CCSprite::create("letters/levelcomplete.png");
			success->setPosition(m_mainpad->getPosition());
			addChild(success, 0, tagLevelFinished);
			createFireworks(m_level / 2 + 2);

			//allow next level after 2 seconds
			m_buttons->setModeNext();
			m_buttons->setStateEnabled(false);
			scheduleOnce(schedule_selector(GameScene::onAllowNext), 3);
			

		}
		else if(m_level == 9)
		{
			CCSprite *success = CCSprite::create("letters/congratulation.png");
			success->setPosition(m_mainpad->getPosition());
			addChild(success, 0, tagLevelFinished);
			createFireworks(10);
		}
		GameSound::sharedGameSound()->playBackgroundMusic(LEVEL_COMPLETE);

		//max score
		if(m_maxScore < m_score)
		{
			m_maxScore = m_score;
			resetMaxScore();
			GameData::sharedGameData()->saveScore(m_maxScore);

			//showing new record crash sprite
			m_cntCrashed ++;
			if(m_cntCrashed == 1)
				showNodeAction(CCSprite::create("letters/newrec.png"), ccpAdd(m_mainpad->getPosition(), ccp(0, 100)), 3, 100);
		}	
	}
	else
	{
		CCSprite *failed = CCSprite::create("letters/failed.png");
		failed->setPosition(m_mainpad->getPosition());
		addChild(failed, 0, tagLevelFinished);

		GameSound::sharedGameSound()->playBackgroundMusic(LEVEL_FAIL);
	}
}
void GameScene::nextLevel()
{
	removeChildByTag(tagLevelFinished, true);
	resetCounters();
}
void GameScene::setRepairMode()
{
	m_mainpad->setRepairMode();
}
void GameScene::removeShowingPipe()
{
	m_showingPipe->removeSpritesFromParent();
}
Buttons* GameScene::getButtons()
{
	return m_buttons;
}
void GameScene::keyBackClicked()
{
	CCDirector::sharedDirector()->replaceScene(LoadingScene::createWithTargetScene(TargetSceneWelcomeScene));
}
