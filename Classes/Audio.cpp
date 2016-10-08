#include "Audio.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

typedef unsigned int uint;

float Audio::_volumeBackground = 1;
float Audio::_volumeEffect     = 1;

const std::string Audio::EffectMusicName = "effect.wav";
const std::string Audio::BackgroundMusicName = "background.mp3";
Audio::auto_call_func Audio::func;  /* call construct */

void Audio::initPreloads()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(Audio::EffectMusicName.c_str());
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(Audio::BackgroundMusicName.c_str());
}

void Audio::PlayMusicEffect()
{
    static uint idEffect = 0;

    if (idEffect != 0)
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(idEffect);

    idEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Audio::EffectMusicName.c_str());
}

void Audio::PlayMusicBackground()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(Audio::BackgroundMusicName.c_str());
}

void Audio::ResumeMusicBackground()
{

}

void Audio::PauseMusicBackground()
{

}

void Audio::SetVolumeMusicBackground(float volume)
{

}

void Audio::SetVolumeMusicEffect(float volume)
{

}

