#include "Mainpad.h"
#include "GameScene.h"
#include "GameSound.h"
#include "GameData.h"
using namespace cocos2d;

Mainpad* Mainpad::createWithParent(CCNode *parent)
{

	Mainpad* object = new Mainpad;
	object->initWithParent(parent);
	object->autorelease();
	return object;
}
Mainpad* Mainpad::initWithParent(CCNode *parent)
{

	//create background and select sprite
	createBackgroundAndSelect(parent);
	
	
	//register touch delegate
	CCTouchDispatcher *touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	touchDispatcher->addTargetedDelegate(this, 1, true);

	m_pipeDict = NULL;
	reset();

	return this;
}
void Mainpad::reset()
{
	m_startPipe = NULL;
	m_bRepairMode = false;
	m_bTouchEnded = true;

	setTouchEnabled(false);
	resetAllPipes();
}
void Mainpad::resetAllPipes()
{
	//if it is already created, remove all elements from the parent and delete array
	if(m_pipeDict)
	{
		CCArray* keys = m_pipeDict->allKeys();
		CCObject* key;
		CCARRAY_FOREACH(keys, key)
		{
			CCString* key_str = (CCString*)key;
			Pipe* pipe = (Pipe*)m_pipeDict->objectForKey(key_str->getCString());
			pipe->removeSpritesFromParent();
		}
		
		m_pipeDict->removeAllObjects();
		m_pipeDict->release();
	}
	m_pipeDict = new CCDictionary;
}
void Mainpad::createBackgroundAndSelect(CCNode *parent)
{
	//create background sprite
	m_backSprite = CCSprite::create("mainpad.png");
	parent->addChild(m_backSprite);

	//init interval distances according to row count and column count
	m_padSize = m_backSprite->getContentSize();
	m_intervalDistanceX = m_padSize.width / COL_COUNT;
	m_intervalDistanceY = m_padSize.height / ROW_COUNT;

	//create rectangle to point the pad when touch moving
	m_selectSprite = CCSprite::create("pad_select.png");
	parent->addChild(m_selectSprite);
	m_selectSprite->setVisible(false);
	m_selectSprite->setAnchorPoint(CCPointMake(0, 0));
	//adjust the size of the rectangle according to the interval distances
	CCSize selectSize = m_selectSprite->getContentSize();
	m_selectSprite->setScaleX(m_intervalDistanceX / selectSize.width);
	m_selectSprite->setScaleY(m_intervalDistanceY / selectSize.height);
}
Mainpad::~Mainpad()
{
	m_pipeDict->removeAllObjects();
	m_pipeDict->release();
}
void Mainpad::destroy()
{
	CCScheduler *scheduler = CCDirector::sharedDirector()->getScheduler();
	scheduler->unscheduleAllSelectorsForTarget(this);

	CCTouchDispatcher *touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	touchDispatcher->removeDelegate(this);
}
bool Mainpad::isTouchForMe(CCTouch *pTouch)
{
	CCPoint touchPos = pTouch->getLocationInView();
	touchPos = CCDirector::sharedDirector()->convertToGL(touchPos);

	CCRect rect = m_backSprite->boundingBox();
	rect.size.width -= 10;
	rect.size.height -= 10;
	return rect.containsPoint(touchPos);
}

void Mainpad::placeSelectionFromTouch(CCTouch *pTouch)
{
	CCPoint position = getSelctedPositionFromTouch(pTouch);
	m_selectSprite->setPosition(position);
}
CCPoint Mainpad::getSelctedPositionFromTouch(CCTouch *pTouch)
{
	CCPoint touchPos = m_backSprite->convertTouchToNodeSpace(pTouch);
	int px = touchPos.x / m_intervalDistanceX;
	int py = touchPos.y / m_intervalDistanceY;
	float posx = int(touchPos.x / m_intervalDistanceX) * m_intervalDistanceX;
	float posy = int(touchPos.y / m_intervalDistanceY) * m_intervalDistanceY;
	CCPoint position = m_backSprite->convertToWorldSpace(CCPointMake(posx, posy));
	return position;
}

Pipe* Mainpad::pipeAtPosition(CCPoint position)
{
	CCString* key = CCString::createWithFormat("%d:%d", (int)position.x, (int)position.y);
	Pipe* pipe = (Pipe*)m_pipeDict->objectForKey(key->getCString());
	if(pipe)
		return pipe;
	else
		return NULL;
}
bool Mainpad::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(!m_bTouchEnabled)
		return false;
	if(!isTouchForMe(pTouch))
		return false;
	if(!m_bTouchEnded)
		return false;
	
	m_bTouchEnded = false;

	//show the rectangle
	m_selectSprite->setVisible(true);
	placeSelectionFromTouch(pTouch);

	return true;
}
void Mainpad::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	//-------if touch is disabled, then hide the selecting sprite------
	if(!m_bTouchEnabled)
	{
		m_selectSprite->setVisible(false);
		return;
	}
	if(m_bTouchEnded)
		return;

	if(isTouchForMe(pTouch))
		placeSelectionFromTouch(pTouch);
}
void Mainpad::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if(!m_bTouchEnabled)
		return;
	if(m_bTouchEnded)
		return;

	m_bTouchEnded = true;
	m_selectSprite->setVisible(false);

	if(isTouchForMe(pTouch))
	{
		CCPoint position = getSelctedPositionFromTouch(pTouch);
		CCPoint offset = CCPointMake(m_intervalDistanceX * 0.5, m_intervalDistanceY * 0.5);
		position = ccpAdd(position, offset);

		
		Pipe *selectedPipe = pipeAtPosition(position);
		
		//in case of normal mode, if there is already a pipe, then quit
		if(!m_bRepairMode && !selectedPipe)
		{
			//create pipe according to the current type
			CCSpriteBatchNode *pipeContainer = GameScene::sharedGameScene()->getPipeContainer();
			Pipe *puttingPipe = Pipe::createWithParentAndCategory(pipeContainer, m_pipeCategoryNumber);
			puttingPipe->setScale(m_intervalDistanceX, m_intervalDistanceY);

			//place the pipe
			puttingPipe->setPosition(position);

			//add the information
			m_pipeDict->setObject(puttingPipe, CCString::createWithFormat("%d:%d", (int)position.x, (int)position.y)->getCString());

			GameScene* gameScene = GameScene::sharedGameScene();
			gameScene->produceRandomPipe();
			
			if(gameScene->getButtons()->m_repairCount >= 1)
				GameScene::sharedGameScene()->getButtons()->setRepairEnabled(true);

			GameSound::sharedGameSound()->playEffect(PIPE_PUT);
		}
		//in case of repair mode, there should be a pipe at the place you selected
		else if(m_bRepairMode && selectedPipe && selectedPipe != m_startPipe)
		{
			GameScene::sharedGameScene()->showCurrentPipeFromCategory(selectedPipe->m_categoryNumber);

			selectedPipe->removeSpritesFromParent();
			m_pipeDict->removeObjectForKey(CCString::createWithFormat("%d:%d", (int)position.x, (int)position.y)->getCString());
			
			m_bRepairMode = false;
		}
		
	}
}
void Mainpad::setPipeType(categories categoryNumber)
{
	m_pipeCategoryNumber = categoryNumber;
}
void Mainpad::start()
{
	CCSpriteBatchNode *pipeContainer = GameScene::sharedGameScene()->getPipeContainer();

	//set the random direction of the starting pipe and create it
	m_startDirection = CCRANDOM_0_1() * 3;
	m_startPipe = Pipe::createStartPipeWithParent(pipeContainer, m_startDirection);
	m_startPipe->setScale(m_intervalDistanceX, m_intervalDistanceY);

	//place the pipe random
	float posx = int(CCRANDOM_0_1() * (COL_COUNT - 2)) + 1.5;
	float posy = int(CCRANDOM_0_1() * (ROW_COUNT - 2)) + 1.5;
	CCPoint position = m_backSprite->convertToWorldSpace(CCPointMake(posx * m_intervalDistanceX, posy * m_intervalDistanceY));
	m_startPipe->setPosition(position);

	//add to the array
	m_pipeDict->setObject(m_startPipe, CCString::createWithFormat("%d:%d", (int)position.x, (int)position.y)->getCString());
	setTouchEnabled(true);
}
void Mainpad::go()
{
	m_startPipe->showPipeFull();
	GameSound::sharedGameSound()->playEffect(PIPE_WATER);

	//to delay half a second
	CCScheduler *scheduler = CCDirector::sharedDirector()->getScheduler();
	scheduler->unscheduleAllSelectorsForTarget(this);
	scheduler->scheduleSelector(schedule_selector(Mainpad::onDelay), this, 0.5f, false);

}
void Mainpad::onDelay(float dt)
{
	//-------------getting next pipe -------------------
	CCPoint startPoint = m_startPipe->getPosition();
	int direct_x = m_startDirection % 2 == 0 ? m_startDirection - 1 : 0;
	int direct_y = m_startDirection % 2 == 0 ? 0 : 2 - m_startDirection;

	startPoint = ccpAdd(startPoint, CCPointMake(direct_x * m_intervalDistanceX, direct_y * m_intervalDistanceY));
	Pipe *nextPipe = pipeAtPosition(startPoint);

	//-----------go processing -------------------------
	if(!isConnected(nextPipe))
		endConnecting();
	else
	{
		m_startPipe = nextPipe;
		nextConnecting();
	}
}
bool Mainpad::isConnected(Pipe* nextPipe)
{
	if(!nextPipe)
		return false;
	else if(m_startDirection == 0 && !nextPipe->m_bRightOpening)
		return false;
	else if(m_startDirection == 1 && !nextPipe->m_bBottomOpening)
		return false;
	else if(m_startDirection == 2 && !nextPipe->m_bLeftOpening)
		return false;
	else if(m_startDirection == 3 && !nextPipe->m_bTopOpening)
		return false;
	return true;
}
void Mainpad::nextConnecting()
{

	if(m_startPipe->m_categoryNumber == categoryAllOpen)
	{
		if(m_startDirection % 2 == 0)
			m_startPipe->m_bHorizontal = true;
		else
			m_startPipe->m_bHorizontal = false;
	}
	else if(m_startPipe->m_bLeftOpening && m_startDirection != 2)
		m_startDirection = 0;
	else if(m_startPipe->m_bTopOpening && m_startDirection != 3)
		m_startDirection = 1;
	else if(m_startPipe->m_bRightOpening && m_startDirection != 0)
		m_startDirection = 2;
	else if(m_startPipe->m_bBottomOpening && m_startDirection != 1)
		m_startDirection = 3;

	showScore();

	//increase the connected pipe count
	GameScene::sharedGameScene()->addGoal(1);

	//next checking
	go();
}
void Mainpad::showScore()
{
	GameScene* gameScene = GameScene::sharedGameScene();
	//if the water turns, then add 30 score, else add 10 score
	if(m_startPipe->m_categoryNumber >= categoryLeftTop)
	{
		gameScene->showNodeAction(CCLabelTTF::create("30", "Arial", 30), m_startPipe->getPosition());
		gameScene->addScore(30);
	}
	else
	{
		gameScene->showNodeAction(CCLabelTTF::create("10", "Arial", 30), m_startPipe->getPosition());
		gameScene->addScore(10);
	}
}
void Mainpad::endConnecting()
{
	CCScheduler *scheduler = CCDirector::sharedDirector()->getScheduler();
	scheduler->unscheduleAllSelectorsForTarget(this);
	setTouchEnabled(false);
	GameScene::sharedGameScene()->checkLevel();
}
void Mainpad::setTouchEnabled(bool flag)
{
	m_bTouchEnabled = flag;
}
CCPoint Mainpad::getPosition()
{
	return m_backSprite->getPosition();
}
CCSize Mainpad::getPadSize()
{
	return m_padSize;
}
void Mainpad::setRepairMode()
{
	m_bRepairMode = true;
	GameScene::sharedGameScene()->removeShowingPipe();
}
void Mainpad::setPosition(CCPoint position)
{
	m_backSprite->setPosition(position);
}