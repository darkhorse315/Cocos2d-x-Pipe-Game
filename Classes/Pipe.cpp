#include "Pipe.h"

using namespace cocos2d;


Pipe::~Pipe(void)
{

}

//-----------------------create start pipe-----------------
Pipe* Pipe::createStartPipeWithParent(CCNode *parent, int direction)
{
	Pipe *object = new Pipe;
	object->initStartPipeWithParent(parent, direction);
	object->autorelease();
	return object;
}
void Pipe::initStartPipeWithParent(CCNode *parent, int direction)
{
	//wing
	m_wingSpr = CCSprite::createWithSpriteFrameName("wings.png");
	parent->addChild(m_wingSpr);
	m_wingSpr->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));

	//pipe sprite and pipe sprite full of water
	m_pipeSprite = CCSprite::createWithSpriteFrameName("pipe_start.png");
	m_pipeSpriteFull = CCSprite::createWithSpriteFrameName("pipe_start_full.png");
	
	parent->addChild(m_pipeSprite, 0, tagStartPipe);
	parent->addChild(m_pipeSpriteFull, 0, tagStartPipeFull);

	
	//rotate by direction
	m_pipeSprite->setRotation(direction * 90);
	m_pipeSpriteFull->setRotation(direction * 90);
	m_pipeSpriteFull->setVisible(false);

	m_categoryNumber = categoryOneOpen;
	
	m_bLeftOpening = m_bRightOpening = m_bTopOpening = m_bBottomOpening = false;
	switch(direction)
	{
	case 0:
		m_bLeftOpening = true;
		break;
	case 1:
		m_bTopOpening = true;
		break;
	case 2:
		m_bRightOpening = true;
		break;
	case 3:
		m_bBottomOpening = true;
		break;
	}
}


//-----------------------create normal pipe-----------------------------
Pipe* Pipe::createWithParentAndCategory(CCNode *parent, categories categoryNumber)
{
	Pipe *object = new Pipe;
	object->initWithParentAndCategory(parent, categoryNumber);
	object->autorelease();
	return object;
}

void Pipe::initWithParentAndCategory(CCNode *parent, categories categoryNumber)
{
	m_pipeSprite = m_pipeSpriteFull = m_pipeSpriteFull_H = m_pipeSpriteFull_V = m_wingSpr = NULL;
	resetPipe(parent, categoryNumber);
	
}
void Pipe::removeSpritesFromParent()
{
	if(m_pipeSprite)
	{
		m_pipeSprite->removeFromParentAndCleanup(true);
		m_pipeSprite = NULL;
		if(m_categoryNumber !=categoryAllOpen)
		{
			m_pipeSpriteFull->removeFromParentAndCleanup(true);
			m_pipeSpriteFull = NULL;
		}
		else
		{
			m_pipeSpriteFull_H->removeFromParentAndCleanup(true);
			m_pipeSpriteFull_V->removeFromParentAndCleanup(true);
			m_pipeSpriteFull_H = m_pipeSpriteFull_V = NULL;
		}
		if(m_wingSpr)
		{
			m_wingSpr->removeFromParentAndCleanup(true);
			m_wingSpr = NULL;
		}
	}
}
void Pipe::resetPipe(CCNode *parent, categories categoryNumber)
{
	removeSpritesFromParent();
	
	m_bLeftOpening = m_bRightOpening  = m_bTopOpening = m_bBottomOpening = false;
	m_categoryNumber = categoryNumber;

	switch(categoryNumber)
	{
	case categoryLeftRight:
		m_pipeSprite = CCSprite::createWithSpriteFrameName("pipe0.png");
		m_pipeSpriteFull = CCSprite::createWithSpriteFrameName("pipe0_full.png");
		m_bLeftOpening = m_bRightOpening = true;
		break;
	case categoryTopBottom:
		m_pipeSprite = CCSprite::createWithSpriteFrameName("pipe0.png");
		m_pipeSpriteFull = CCSprite::createWithSpriteFrameName("pipe0_full.png");
		m_pipeSprite->setRotation(90);
		m_bTopOpening = m_bBottomOpening = true;
		break;
	case categoryAllOpen:
		m_pipeSprite = CCSprite::createWithSpriteFrameName("pipe2.png");
		m_pipeSpriteFull_H = CCSprite::createWithSpriteFrameName("pipe2_full.png");
		m_pipeSpriteFull_V = CCSprite::createWithSpriteFrameName("pipe2_full.png");
		m_pipeSpriteFull_V->setRotation(90);
		m_bLeftOpening = m_bRightOpening = m_bTopOpening = m_bBottomOpening = true;
		break;
	case categoryLeftTop:
		m_pipeSprite = CCSprite::createWithSpriteFrameName("pipe1.png");
		m_pipeSpriteFull = CCSprite::createWithSpriteFrameName("pipe1_full.png");
		m_bLeftOpening = m_bTopOpening = true;
		break;
	case categoryRightTop:
		m_pipeSprite = CCSprite::createWithSpriteFrameName("pipe1.png");
		m_pipeSpriteFull = CCSprite::createWithSpriteFrameName("pipe1_full.png");
		m_pipeSprite->setRotation(90);
		m_bRightOpening = m_bTopOpening = true;
		break;
	case categoryRightBottom:
		m_pipeSprite = CCSprite::createWithSpriteFrameName("pipe1.png");
		m_pipeSpriteFull = CCSprite::createWithSpriteFrameName("pipe1_full.png");
		m_pipeSprite->setRotation(180);
		m_bRightOpening = m_bBottomOpening = true;
		break;
	case categoryLeftBottom:
		m_pipeSprite = CCSprite::createWithSpriteFrameName("pipe1.png");
		m_pipeSpriteFull = CCSprite::createWithSpriteFrameName("pipe1_full.png");
		m_pipeSprite->setRotation(270);
		m_bLeftOpening = m_bBottomOpening = true;
		break;
	default:
		return;
	}

	if(categoryNumber != categoryAllOpen)
	{
		parent->addChild(m_pipeSpriteFull);
		m_pipeSpriteFull->setRotation(m_pipeSprite->getRotation());
		m_pipeSpriteFull->setVisible(false);
	}
	else
	{
		parent->addChild(m_pipeSpriteFull_H);
		parent->addChild(m_pipeSpriteFull_V);
		m_pipeSpriteFull_H->setVisible(false);
		m_pipeSpriteFull_V->setVisible(false);
	}

	parent->addChild(m_pipeSprite);

}
void Pipe::setPosition(CCPoint position)
{
	m_pipeSprite->setPosition(position);
	if(m_categoryNumber != categoryAllOpen)
		m_pipeSpriteFull->setPosition(position);
	else
	{
		m_pipeSpriteFull_H->setPosition(position);
		m_pipeSpriteFull_V->setPosition(position);
	}
	if(m_wingSpr)
	{
		CCSize size = m_pipeSprite->getContentSize();
		m_wingSpr->setPosition(m_pipeSprite->convertToWorldSpace(ccp(size.width * 0.5 + 8, size.height * 0.5)));
	}
}
CCPoint Pipe::getPosition()
{
	return m_pipeSprite->getPosition();
}
void Pipe::showPipeFull()
{
	if(m_categoryNumber != categoryAllOpen)
		m_pipeSpriteFull->setVisible(true);
	else
	{
		if(m_bHorizontal)
			m_pipeSpriteFull_H->setVisible(true);
		else
			m_pipeSpriteFull_V->setVisible(true);
	}

}

categories Pipe::getRandomCategory()
{
	int randomNumber = CCRANDOM_0_1() * PIPE_CATEGORY_COUNT;
	
	categories categoryNumber;
	switch (randomNumber)
	{
	case categoryAllOpen:
		categoryNumber = categoryAllOpen;
		break;
	case categoryLeftBottom:
		categoryNumber = categoryLeftBottom;
		break;
	case categoryLeftTop:
		categoryNumber = categoryLeftTop;
		break;
	case categoryLeftRight:
		categoryNumber = categoryLeftRight;
		break;
	case categoryRightBottom:
		categoryNumber = categoryRightBottom;
		break;
	case categoryRightTop:
		categoryNumber = categoryRightTop;
		break;
	case categoryTopBottom:
		categoryNumber = categoryTopBottom;
		break;
	default:
		break;
	}
	return categoryNumber;
}
void Pipe::setScale(int toWidth, int toHeight)
{
	CCSize pipeSize = m_pipeSprite->getContentSize();
	float scaleX = toWidth / pipeSize.width;
	float scaleY = toHeight / pipeSize.height;

	m_pipeSprite->setScaleX(scaleX);
	m_pipeSprite->setScaleY(scaleY);

	if(m_categoryNumber != categoryAllOpen)
	{
		m_pipeSpriteFull->setScaleX(scaleX);
		m_pipeSpriteFull->setScaleY(scaleY);
	}
	else
	{
		m_pipeSpriteFull_H->setScaleX(scaleX);
		m_pipeSpriteFull_H->setScaleY(scaleY);
		m_pipeSpriteFull_V->setScaleX(scaleX);
		m_pipeSpriteFull_V->setScaleY(scaleY);
	}
}
