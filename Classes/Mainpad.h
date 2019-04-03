#pragma once
#include "cocos2d.h"
#include "Pipe.h"

#define ROW_COUNT 8
#define COL_COUNT 12

class Mainpad : public cocos2d::CCObject, public cocos2d::CCTargetedTouchDelegate
{
public:
	~Mainpad();
	void destroy();

	//----------------create and init
	static Mainpad* createWithParent(cocos2d::CCNode *parent);
	Mainpad* initWithParent(cocos2d::CCNode *parent);

	//--------------touch event--------------------
	virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	void setTouchEnabled(bool flag);
	bool isTouchForMe(cocos2d::CCTouch *pTouch);

	//----------------reset the game---------------
	void reset();
	

	//------------game logic------------
	void start();
	void onDelay(float dt);
	void go();
	void endConnecting();
	bool isConnected(Pipe*);
	void nextConnecting();
	void showScore();

	//--------------pipe------------
	void setPipeType(categories categoryNumber);
	void resetAllPipes();

	cocos2d::CCPoint getPosition();
	cocos2d::CCSize getPadSize();
private:
	bool m_bTouchEnabled;
	bool m_bTouchEnded;

	//------------sprites-----------
	cocos2d::CCSprite *m_backSprite;
	cocos2d::CCSprite *m_selectSprite;

	//-----------
	float m_intervalDistanceX;
	float m_intervalDistanceY;
	cocos2d::CCSize m_padSize;

	//--------------pipe--------
	categories m_pipeCategoryNumber;
	//cocos2d::CCArray *m_pipeArray;
	cocos2d::CCDictionary *m_pipeDict;
	Pipe* m_startPipe;
	int m_startDirection;
	
	//---------------repair mode---------
	bool m_bRepairMode;

public:
	void placeSelectionFromTouch(cocos2d::CCTouch *pTouch);
	cocos2d::CCPoint getSelctedPositionFromTouch(cocos2d::CCTouch *pTouch);
	Pipe* pipeAtPosition(cocos2d::CCPoint position);

	
	void createBackgroundAndSelect(cocos2d::CCNode* parent);
	
	void setRepairMode();
	void setPosition(cocos2d::CCPoint position);
};
