#pragma once


#include <engine/component.h>

class Transform;

class LoadingBar : public Component
{
public:
    LoadingBar();

    void Start() override;

    ReflectiveData GetReflectiveData() override;
    void SetFillAmount(float fillAmount);
    float GetFillAmount();

private:
    void UpdateGraphics();

    float fillAmount = 0;
    std::weak_ptr<Transform> fillBar;
};
