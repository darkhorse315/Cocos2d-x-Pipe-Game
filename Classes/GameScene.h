#pragma once
#include "Mainpad.h"
#include "cocos2d.h"
#include "buttons.h"
enum TAGS{
	tagScoreLabel,
	tagLevelLabel,
	tagGoalLabel,
	tagMaxScoreLabel,
	tagLevelFinished,
};
class GameScene : public cocos2d::CCLayer
{
public:
	~GameScene();
	virtual bool init();
	static cocos2d::CCScene* scene();
	static GameScene* sharedGameScene();
	
	// implement the "static node()" method manually
	CREATE_FUNC(GameScene);

	virtual void keyBackClicked();
private:
	int m_cntCrashed;
	int m_time;
	int m_pipeTotalCount;
	int m_pipeConnectedCount;
	int m_score;
	int m_maxScore;
	int m_level;
	Mainpad *m_mainpad;
	Pipe *m_showingPipe;
	Buttons* m_buttons;
	cocos2d::CCSpriteBatchNode *m_pipeBatchNode;
	cocos2d::CCSprite* m_timeCounter;
public:
	void createBG();
	void createMainpad();
	void createPipeContainer();
	void createButtons();
	void createCounters();
	void initVariables();
	void resetCounters();

	//-------------time counter---------------
	void onPrevStart(float dt);
	void onStart(float dt);

	//----------start the game and start checking when it's over
	void startTheGame();
	void startCheckingTheGame();

	void addScore(int plusScore);
	void addGoal(int plusGoal);
	void addLevel(int plusLevel);

	void resetMaxScore();

	void checkLevel();
	void nextLevel();
	void onAllowNext(float dt);
	
	//--------show hint string for a while---------
	void showNodeAction(cocos2d::CCSprite* node, cocos2d::CCPoint position, int delay = 1, int zorder = 0);
	void onShowNodeFinished(cocos2d::CCNode* node);
	
	//-----------create firework----------
	void createFireworks(int cnt);
	void onCreateFirework(float dt);

	void setRepairMode();

	//-----------set the current category number and show it
	void showCurrentPipeFromCategory(categories categoryNumber);
	void produceRandomPipe();
	void removeShowingPipe();
	
	Buttons* getButtons();
	cocos2d::CCSpriteBatchNode* getPipeContainer();


};
