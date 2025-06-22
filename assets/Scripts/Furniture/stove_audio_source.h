#pragma once


#include <engine/component.h>

class AudioSource;

class StoveAudioSource : public Component
{
public:
    StoveAudioSource();

    ReflectiveData GetReflectiveData() override;

    void StartStove();
    void EndStove();
    std::weak_ptr<AudioSource> audioSource;

private:
    void CheckAudio();

    int usedStoveCount = 0;
};
