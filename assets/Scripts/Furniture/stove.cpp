#include "stove.h"
#include <xenity.h>
#include "../game_manager.h"
#include "../Player/player.h"
#include "../Item/ingredient.h"
#include "../Item/item_builder.h"
#include "../loading_bar.h"
#include "stove_audio_source.h"

/**
 * Constructor
 */
Stove::Stove()
{
}

/**
 * Called once when the script is enabled
 */
void Stove::Start()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[Stove::Start] gameManager is not filled!");
        return;
    }

    particleSystem.lock()->SetIsEmitting(false);
}

/**
 * Called every frame
 */
void Stove::Update()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[Stove::Update] gameManager is not filled!");
        return;
    }

    // If a ingredient is placed and not fully cooked
    if (cookingIngredient.lock() && cookingState != CookingState::Overcooked)
    {
        timer -= Time::GetDeltaTime();
        loadingBar.lock()->SetFillAmount(1 - (timer / cookTime));
        if (timer <= 0)
        {
            // Change ingredient cooking state
            timer = cookTime;

            IngredientType oldType = cookingIngredient.lock()->ingredientType;
            IngredientType type = IngredientType::None;
            if (oldType == IngredientType::UncookedSteak)
            {
                type = IngredientType::CookedSteak;
                cookingState = CookingState::Cooked;
                loadingBar.lock()->GetGameObject()->SetActive(false);
            }
            else if (oldType == IngredientType::CookedSteak)
            {
                type = IngredientType::OvercookedSteak;
                cookingState = CookingState::Overcooked;
            }

            std::shared_ptr<GameObject> newGo = gameManager.lock()->itemBuilder.lock()->CreateIngredient(type);
            newGo->SetParent(ingredientPosition.lock());
            newGo->GetTransform()->SetLocalPosition(Vector3(0));
            newGo->GetTransform()->SetLocalEulerAngles(Vector3(0));

            Destroy(cookingIngredient.lock()->GetGameObject());
            cookingIngredient = newGo->GetComponent<Ingredient>();
        }
    }
}

void Stove::PlaceTake(std::shared_ptr<Player> player)
{
    std::shared_ptr<GameObject> heldItem = player->GetHeldItem();
    if (heldItem)
    {
        if (!cookingIngredient.lock())
        {
            // Place ingredient
            std::shared_ptr<Ingredient> ingredientToPut = heldItem->GetComponent<Ingredient>();
            if (ingredientToPut && ingredientToPut->ingredientType == IngredientType::UncookedSteak)
            {
                cookingIngredient = ingredientToPut;
                ingredientToPut->GetGameObject()->SetParent(ingredientPosition.lock());
                ingredientToPut->GetGameObject()->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
                ingredientToPut->GetGameObject()->GetTransform()->SetLocalEulerAngles(Vector3(0, 0, 0));
                timer = cookTime;
                player->RemoveHeldItem();
                loadingBar.lock()->GetGameObject()->SetActive(true);
                loadingBar.lock()->SetFillAmount(0);
                stoveAudioSource.lock()->StartStove();
                particleSystem.lock()->SetIsEmitting(true);
            }
        }
    }
    else
    {
        // Take ingredient
        if (cookingIngredient.lock())
        {
            cookingState = CookingState::NotCooked;
            player->SetHeldItem(cookingIngredient.lock()->GetGameObject());
            cookingIngredient.reset();
            loadingBar.lock()->GetGameObject()->SetActive(false);
            stoveAudioSource.lock()->EndStove();
            particleSystem.lock()->SetIsEmitting(false);
        }
    }
}

void Stove::OnTriggerEnter(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(std::dynamic_pointer_cast<Interactable>(shared_from_this()));
}

void Stove::OnTriggerExit(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(nullptr);
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData Stove::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(triggers, true);
    ADD_VARIABLE(gameManager, true);
    ADD_VARIABLE(ingredientPosition, true);
    ADD_VARIABLE(loadingBar, true);
    ADD_VARIABLE(cookTime, true);
    ADD_VARIABLE(stoveAudioSource, true);
    ADD_VARIABLE(particleSystem, true);
    END_REFLECTION();
}
