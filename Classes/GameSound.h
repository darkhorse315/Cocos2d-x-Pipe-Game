#pragma once
#include "cocos2d.h"


enum BACKGROUND_MUSIC{
	WELCOME_MUSIC,
	GAME_MUSIC,
	LEVEL_FAIL,
	LEVEL_COMPLETE
};
enum EFFECT{
	BUTTON_CLICKED,
	PIPE_PUT,
	PIPE_WATER
};
typedef BACKGROUND_MUSIC background_music;
typedef EFFECT effect;

class GameSound
{
public:
	GameSound(void);
	~GameSound(void);
	void playBackgroundMusic(background_music backMusic);
	void resumeBackgroundMusic();
	void playEffect(effect effectSound);

	void stopAllEffects();

	void switchSound();
	void preloadSounds();

	static void create();
	static GameSound* sharedGameSound();
private:
	bool m_bSoundTurnedOn;
	
};
