#include "auto_destroy.h"
#include <xenity.h>

/**
 * Constructor
 */
AutoDestroy::AutoDestroy()
{
}

/**
 * Called once when the script is enabled
 */
void AutoDestroy::Start()
{
    audioSource.lock()->Play();
}

/**
 * Called every frame
 */
void AutoDestroy::Update()
{
    timer -= Time::GetDeltaTime();
    if (timer <= 0)
    {
        timer = 1000000;
        Destroy(GetGameObject());
    }
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData AutoDestroy::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(audioSource);
    END_REFLECTION();
}
