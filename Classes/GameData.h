#pragma once

#include "cocos2d.h"
class GameData
{
public:
	GameData(void);
	~GameData(void);

	static void create();
	static GameData* sharedGameData();
	void loadData();
	void saveScore(int score);
	void saveSoundState(bool turnedOn);

public:

	bool isSoundTurnedOn;
	int maxScore;

};
cocos2d::CCPoint getPos(float posx, float posy);
