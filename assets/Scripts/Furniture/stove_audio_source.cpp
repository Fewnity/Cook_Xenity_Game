#include "stove_audio_source.h"
#include <xenity.h>

/**
 * Constructor
 */
StoveAudioSource::StoveAudioSource()
{
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData StoveAudioSource::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(audioSource, true);
    END_REFLECTION();
}

void StoveAudioSource::StartStove()
{
    usedStoveCount++;
    CheckAudio();
}

void StoveAudioSource::EndStove()
{
    usedStoveCount--;
    CheckAudio();
}

void StoveAudioSource::CheckAudio()
{
    if (usedStoveCount == 1)
    {
        audioSource.lock()->Play();
    }
    else if (usedStoveCount == 0)
    {
        audioSource.lock()->Stop();
    }
}
