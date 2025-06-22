#include "crate.h"
#include <xenity.h>
#include "../game_manager.h"
#include "../Player/player.h"
#include "../Item/ingredient.h"

/**
 * Constructor
 */
Crate::Crate()
{
}

/**
 * Called once when the script is enabled
 */
void Crate::Start()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[Crate::Start] gameManager is not filled!");
        return;
    }
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData Crate::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(triggers, true);
    ADD_VARIABLE(gameManager, true);
    ADD_VARIABLE(ingredientToSpawn, true);
    ADD_VARIABLE(ingredientType, true);
    ADD_VARIABLE(audioSource, true);
    END_REFLECTION();
}

void Crate::PlaceTake(std::shared_ptr<Player> player)
{
    std::shared_ptr<GameObject> heldItem = player->GetHeldItem();
    if (heldItem)
    {
        // If the held ingredient is of the same type of the create, replace the ingredient
        std::shared_ptr<Ingredient> ingredient = heldItem->GetComponent<Ingredient>();
        if (ingredient && ingredient->ingredientType == ingredientType)
        {
            player->RemoveHeldItem();
            Destroy(heldItem);
        }
    }
    else
    {
        // Pick a ingredient
        std::shared_ptr<GameObject> go = Instantiate(ingredientToSpawn.lock());
        player->SetHeldItem(go);
        audioSource.lock()->Play();
    }
}

void Crate::OnTriggerEnter(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(std::dynamic_pointer_cast<Interactable>(shared_from_this()));
}

void Crate::OnTriggerExit(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(nullptr);
}