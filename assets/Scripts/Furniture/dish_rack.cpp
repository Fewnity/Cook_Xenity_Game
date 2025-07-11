#include "dish_rack.h"
#include <xenity.h>
#include "../game_manager.h"
#include "../Player/player.h"
#include "../Item/dish.h"

/**
 * Constructor
 */
DishRack::DishRack()
{
}

/**
 * Called once when the script is enabled
 */
void DishRack::Start()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[DishRack::Start] gameManager is not filled!");
        return;
    }

    currentDishCount = dishCount;
    UpdateGraphics();
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData DishRack::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(triggers);
    ADD_VARIABLE(gameManager);
    ADD_VARIABLE(dishPlate);
    ADD_VARIABLE(dishPlates);
    END_REFLECTION();
}

void DishRack::PlaceTake(std::shared_ptr<Player> player)
{
    std::shared_ptr<GameObject> heldItem = player->GetHeldItem();
    if (heldItem)
    {
        // Place dish
        std::shared_ptr<Dish> dish = heldItem->GetComponent<Dish>();
        if (dish && dish->mealType == MealType::None && !dish->isDirty)
        {
            player->RemoveHeldItem();
            Destroy(heldItem);
            currentDishCount++;
        }
    }
    else
    {
        // Pick dish
        if (currentDishCount > 0)
        {
            std::shared_ptr<GameObject> go = Instantiate(dishPlate.lock());
            player->SetHeldItem(go);
            currentDishCount--;
        }
    }
    UpdateGraphics();
}

void DishRack::UpdateGraphics()
{
    for (int i = 0; i < dishCount; i++)
    {
        dishPlates[i].lock()->SetActive(i < currentDishCount);
    }
}

void DishRack::OnTriggerEnter(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(std::dynamic_pointer_cast<Interactable>(shared_from_this()));
}

void DishRack::OnTriggerExit(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(nullptr);
}