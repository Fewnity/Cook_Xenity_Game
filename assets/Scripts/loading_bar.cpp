#include "loading_bar.h"
#include <xenity.h>

/**
 * Constructor
 */
LoadingBar::LoadingBar()
{
}

/**
 * Called once when the script is enabled
 */
void LoadingBar::Start()
{
    UpdateGraphics();
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData LoadingBar::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(fillBar, true);
    ADD_VARIABLE(fillAmount, true);
    END_REFLECTION();
}

void LoadingBar::SetFillAmount(float fillAmount)
{
    if (fillAmount < 0)
        fillAmount = 0;
    else if (fillAmount > 1)
        fillAmount = 1;

    this->fillAmount = fillAmount;

    UpdateGraphics();
}

float LoadingBar::GetFillAmount()
{
    return fillAmount;
}

void LoadingBar::UpdateGraphics()
{
    if (fillBar.lock())
        fillBar.lock()->SetLocalScale(Vector3(fillAmount, 1, 1));
}
