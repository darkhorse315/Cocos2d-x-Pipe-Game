#include "GameData.h"
using namespace cocos2d;

static GameData* sharedGDInstance = NULL;
GameData::GameData(void)
{
	loadData();
}

GameData::~GameData(void)
{
}
void GameData::create()
{
	GameData* gd = new GameData;
	sharedGDInstance = gd;
}
GameData* GameData::sharedGameData()
{
	if(!sharedGDInstance)
		create();
	return sharedGDInstance;
}
void GameData::saveScore(int score)
{
	maxScore = score;

	FILE* fp = fopen("/data/data/org.cocos2dx.pipe/history.txt", "w");	
	fprintf(fp, "%d %c", maxScore, isSoundTurnedOn? '1' : '0');
	fclose(fp);
}
void GameData::saveSoundState(bool turnedOn)
{
	isSoundTurnedOn = turnedOn;

	FILE* fp = fopen("/data/data/org.cocos2dx.pipe/history.txt", "w");	
	fprintf(fp, "%d %c", maxScore, isSoundTurnedOn? '1' : '0');
	fclose(fp);
}
void GameData::loadData()
{
	//--------
	FILE* fp = fopen("/data/data/org.cocos2dx.pipe/history.txt", "r");
	if(!fp)
	{
		maxScore = 0;
		isSoundTurnedOn = true;
	}	
	else
	{
		char flag;

		fscanf(fp, "%d %c", &maxScore, &flag);
		
		if(flag == '1')
			isSoundTurnedOn = true;
		else
			isSoundTurnedOn = false;
		fclose(fp);
	}
}
CCPoint getPos(float posx, float posy)
{
	return ccp(posx / 1.44, posy / 1.51);
}
