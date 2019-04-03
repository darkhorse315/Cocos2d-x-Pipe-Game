#include "GameSound.h"
#include "SimpleAudioEngine.h"
#include "GameData.h"

using namespace cocos2d;
using namespace CocosDenshion;

static GameSound* sharedInstance = NULL;

GameSound::GameSound(void)
{
	m_bSoundTurnedOn = GameData::sharedGameData()->isSoundTurnedOn;
	preloadSounds();
}

GameSound::~GameSound(void)
{
	SimpleAudioEngine::sharedEngine()->end();
}
void GameSound::playBackgroundMusic(background_music backMusic)
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	if(backMusic == WELCOME_MUSIC)
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("welcome_music.ogg", true);
	else if(backMusic == LEVEL_FAIL)
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("level_fail.mp3");
	else if(backMusic == LEVEL_COMPLETE)
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("level_complete.mp3");
	if(!m_bSoundTurnedOn)
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}
void GameSound::resumeBackgroundMusic()
{
	if(m_bSoundTurnedOn)
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
void GameSound::switchSound()
{
	m_bSoundTurnedOn = !m_bSoundTurnedOn;
	if(m_bSoundTurnedOn)
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->stopAllEffects();
	}
	GameData::sharedGameData()->saveSoundState(m_bSoundTurnedOn);
		
}
void GameSound::playEffect(effect effectSound)
{
	if(!m_bSoundTurnedOn)
		return;
	switch(effectSound)
	{
	case BUTTON_CLICKED:
		SimpleAudioEngine::sharedEngine()->playEffect("button_clicked.mp3");
		break;
	case PIPE_PUT:
		SimpleAudioEngine::sharedEngine()->playEffect("pipe_put.mp3");
		break;
	case PIPE_WATER:
		SimpleAudioEngine::sharedEngine()->playEffect("water.mp3");
		break;
	}
}
void GameSound::preloadSounds()
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("welcome_music.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("level_complete.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("level_fail.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("button_clicked.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("pipe_put.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("water.mp3");
}
void GameSound::create()
{
	GameSound* gs = new GameSound;
	sharedInstance = gs;
}
void GameSound::stopAllEffects()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}
GameSound* GameSound::sharedGameSound()
{
	return sharedInstance;
}
