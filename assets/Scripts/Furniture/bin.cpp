#include "bin.h"
#include <xenity.h>
#include "../game_manager.h"
#include "../Player/player.h"
#include "../Item/dish.h"
#include "../Item/ingredient.h"

/**
 * Constructor
 */
Bin::Bin()
{
}

/**
 * Called once when the script is enabled
 */
void Bin::Start()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[Bin::Start] gameManager is not filled!");
        return;
    }
}

/**
 * Called every frame
 */
void Bin::Update()
{
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData Bin::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(triggers);
    ADD_VARIABLE(gameManager);
    END_REFLECTION();
}

void Bin::PlaceTake(std::shared_ptr<Player> player)
{
    // If the player holds an ingredient, destroy the ingredient
    // If the player holds a dish, empty the dish
    std::shared_ptr<GameObject> heldItem = player->GetHeldItem();
    if (heldItem)
    {
        std::shared_ptr<Ingredient> ingredient = heldItem->GetComponent<Ingredient>();
        std::shared_ptr<Dish> dish = heldItem->GetComponent<Dish>();
        if (ingredient)
        {
            player->RemoveHeldItem();
            Destroy(heldItem);
        }
        else if (dish)
        {
            dish->RemoveIngredients();
        }
    }
}

void Bin::OnTriggerEnter(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(std::dynamic_pointer_cast<Interactable>(shared_from_this()));
}

void Bin::OnTriggerExit(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(nullptr);
}