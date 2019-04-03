#pragma once
#include "cocos2d.h"
#define PIPE_CATEGORY_COUNT 7
typedef enum{
	categoryLeftRight = 0,
	categoryTopBottom = 1,
	categoryAllOpen = 2,
	categoryLeftTop = 3,
	categoryRightTop = 4,
	categoryRightBottom = 5,
	categoryLeftBottom = 6,
	categoryOneOpen = 7
}categories;

enum TAGSTARTPIPE{
	tagStartPipe,
	tagStartPipeFull
};
class Pipe : public cocos2d::CCObject
{
public:
	~Pipe(void);
	//create start pipe
	static Pipe* createStartPipeWithParent(cocos2d::CCNode *parent, int direction);
	void initStartPipeWithParent(cocos2d::CCNode* parent, int direction);

	//create normal pipe
	static Pipe* createWithParentAndCategory(cocos2d::CCNode *parent, categories categoryNumber);
	void initWithParentAndCategory(cocos2d::CCNode *parent, categories categoryNumber);
	//
	void resetPipe(cocos2d::CCNode *parent, categories categoryNumber);
	
	//in terms of position
	void setPosition(cocos2d::CCPoint position);
	cocos2d::CCPoint getPosition();

	//scale
	void setScale(int toWidth, int toHeight);

	//switch to full mode
	void showPipeFull();
private:
	cocos2d::CCSprite *m_pipeSprite;
	cocos2d::CCSprite *m_pipeSpriteFull;
	//for categoryAllOpen
	cocos2d::CCSprite *m_pipeSpriteFull_H;
	cocos2d::CCSprite *m_pipeSpriteFull_V;
	//for start pipe
	cocos2d::CCSprite* m_wingSpr;
public:
	//for categoryAllOpen when the water is passing by it
	bool m_bHorizontal;
	//category number
	categories m_categoryNumber;
	bool m_bLeftOpening, m_bRightOpening, m_bTopOpening, m_bBottomOpening;
	void removeSpritesFromParent();


	static categories getRandomCategory();
};
