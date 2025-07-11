#include "counter.h"
#include <xenity.h>
#include "../game_manager.h"
#include "../Player/player.h"
#include "../Item/ingredient.h"
#include "../Item/dish.h"
#include "../Item/item_builder.h"
#include "../loading_bar.h"

/**
 * Constructor
 */
Counter::Counter()
{
}

/**
 * Called once when the script is enabled
 */
void Counter::Start()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[DishRack::Start] gameManager is not filled! Gameobject: " + GetGameObject()->GetName());
        return;
    }
    if (!middlePosition.lock())
    {
        Debug::PrintError("[DishRack::Start] middlePosition is not filled! Gameobject: " + GetGameObject()->GetName());
        return;
    }
}

/**
 * Called every frame
 */
void Counter::Update()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[Counter::Update] gameManager is not filled!");
        return;
    }

    // If the player is using the counter
    if (isUsed)
    {
        useTimer -= Time::GetDeltaTime();
        loadingBar.lock()->SetFillAmount(1 - (useTimer / cutTime));

        // Apply the action at the end of the timer
        if (useTimer <= 0)
        {
            isUsed = false;
            loadingBar.lock()->GetGameObject()->SetActive(false);
            if (itemOnTop.lock())
            {
                std::shared_ptr<Ingredient> topIngredient = itemOnTop.lock()->GetComponent<Ingredient>();
                if (topIngredient)
                {
                    // Transform the old ingredient to the new one
                    const IngredientType oldType = topIngredient->ingredientType;
                    IngredientType type = IngredientType::None;
                    if (oldType == IngredientType::Tomato)
                        type = IngredientType::TomatoSlice;
                    else if (oldType == IngredientType::CheesePiece)
                        type = IngredientType::CheeseSlice;

                    if (type != IngredientType::None)
                    {
                        std::shared_ptr<GameObject> newGo = gameManager.lock()->itemBuilder.lock()->CreateIngredient(type);
                        newGo->SetParent(middlePosition.lock());
                        newGo->GetTransform()->SetLocalPosition(Vector3(0));
                        newGo->GetTransform()->SetLocalEulerAngles(Vector3(0));
                        Destroy(topIngredient->GetGameObject());
                        itemOnTop = newGo;
                    }
                }
            }
        }
    }
}

void Counter::Interact(std::shared_ptr<Player> player)
{
    if (itemOnTop.lock())
    {
        std::shared_ptr<Ingredient> topIngredient = itemOnTop.lock()->GetComponent<Ingredient>();
        if (topIngredient)
        {
            if (topIngredient->cuttable)
            {
                isUsed = true;
                useTimer = cutTime;
                loadingBar.lock()->GetGameObject()->SetActive(true);
                loadingBar.lock()->SetFillAmount(0);
            }
        }
    }
}

void Counter::StopInteract()
{
    isUsed = false;
    loadingBar.lock()->GetGameObject()->SetActive(false);
}

void Counter::PlaceTake(std::shared_ptr<Player> player)
{
    std::shared_ptr<GameObject> heldItem = player->GetHeldItem();
    if (heldItem) // Place the item
    {
        if (itemOnTop.lock())
        {
            // If there already is a dish, and the player is holding an ingredient, add the ingredient to the dish
            std::shared_ptr<Ingredient> ingredient = heldItem->GetComponent<Ingredient>();
            std::shared_ptr<Dish> topDish = itemOnTop.lock()->GetComponent<Dish>();
            if (topDish && ingredient)
            {
                bool added = topDish->AddIngredient(ingredient);
                if (added)
                {
                    player->RemoveHeldItem();
                }
            }
        }
        else
        {
            // Place the ingredient or the dish on the counter
            std::shared_ptr<Ingredient> ingredient = heldItem->GetComponent<Ingredient>();
            std::shared_ptr<Dish> dish = heldItem->GetComponent<Dish>();
            if (ingredient || dish)
            {
                heldItem->SetParent(middlePosition.lock());
                heldItem->GetTransform()->SetLocalPosition(Vector3(0));
                heldItem->GetTransform()->SetLocalEulerAngles(Vector3(0));
                player->RemoveHeldItem();
                itemOnTop = heldItem;
            }
        }
    }
    else // Take the item
    {
        if (itemOnTop.lock())
        {
            player->SetHeldItem(itemOnTop.lock());
            itemOnTop.reset();
        }
    }
}

void Counter::OnTriggerEnter(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(std::dynamic_pointer_cast<Interactable>(shared_from_this()));
}

void Counter::OnTriggerExit(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(nullptr);
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData Counter::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(triggers);
    ADD_VARIABLE(gameManager);
    ADD_VARIABLE(middlePosition);
    ADD_VARIABLE(loadingBar);
    ADD_VARIABLE(cutTime);
    END_REFLECTION();
}
