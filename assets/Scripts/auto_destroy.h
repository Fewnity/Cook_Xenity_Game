#pragma once


#include <engine/component.h>

class AudioSource;

class AutoDestroy : public Component
{
public:
    AutoDestroy();

    void Start() override;
    void Update() override;

    ReflectiveData GetReflectiveData() override;
    float timer = 2;
    std::weak_ptr<AudioSource> audioSource;

private:
};
