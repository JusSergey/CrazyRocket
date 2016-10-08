#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "cocos2d.h"

class Audio
{
public:
    static void initPreloads();

public:
    static void PlayMusicEffect();
    static void PlayMusicBackground();
    static void ResumeMusicBackground();
    static void PauseMusicBackground();

    static void SetVolumeMusicBackground(float volume);
    static void SetVolumeMusicEffect(float volume);

    inline static float GetVolumeMusicBackground()  { return Audio::_volumeBackground; }
    inline static float GetVolumeMusicEffect()      { return Audio::_volumeEffect;     }

public:
    static const std::string EffectMusicName;
    static const std::string BackgroundMusicName;

private:
    static float _volumeEffect;
    static float _volumeBackground;

    struct auto_call_func {auto_call_func(){ Audio::initPreloads(); }};
    static auto_call_func func;

};

#endif // __AUDIO_H__
