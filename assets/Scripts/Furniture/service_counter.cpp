#include "service_counter.h"
#include <xenity.h>
#include "../game_manager.h"
#include "../Player/player.h"
#include "../Item/dish.h"
#include "../Item/item_builder.h"

/**
 * Constructor
 */
ServiceCounter::ServiceCounter()
{
}

/**
 * Called once when the script is enabled
 */
void ServiceCounter::Start()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[ServiceCounter::Start] gameManager is not filled! Gameobject: " + GetGameObject()->GetName());
        return;
    }

    const int dishPositionsCount = dishPositions.size();
    for (int i = 0; i < dishPositionsCount; i++)
    {
        placedDishes.push_back(DishSlot());
    }
}

/**
 * Called every frame
 */
void ServiceCounter::Update()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[ServiceCounter::Update] gameManager is not filled!");
        return;
    }

    // Wait before deleting placed dishes
    const size_t placedDishesCount = placedDishes.size();
    for (size_t i = 0; i < placedDishesCount; i++)
    {
        DishSlot &slot = placedDishes[i];
        if (slot.placedDish.lock() && slot.isDirty)
        {
            slot.removeTimer -= Time::GetDeltaTime();
            if (slot.removeTimer <= 0)
            {
                dishCountToReturn++;
                Destroy(slot.placedDish.lock());
            }
        }
    }

    // Wait before returning emptied dishes
    if (dishCountToReturn != 0)
    {
        timer -= Time::GetDeltaTime();
        if (timer <= 0)
        {
            int placeIndex = FindFreePlace();
            if (placeIndex != -1)
            {
                dishCountToReturn--;
                timer = 5;
                std::shared_ptr<GameObject> newDis = gameManager.lock()->itemBuilder.lock()->CreateDish(true);
                newDis->SetParent(dishPositions[placeIndex].lock());
                newDis->GetTransform()->SetLocalPosition(Vector3(0));
                newDis->GetTransform()->SetLocalEulerAngles(Vector3(0));
                placedDishes[placeIndex].placedDish = newDis;
                placedDishes[placeIndex].isDirty = false;
                popAudioSource.lock()->Play();
            }
            else
            {
                timer = 1.0f;
            }
        }
    }
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData ServiceCounter::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(triggers, true);
    ADD_VARIABLE(gameManager, true);
    ADD_VARIABLE(dishPositions, true);
    ADD_VARIABLE(bellAudioSource, true);
    ADD_VARIABLE(popAudioSource, true);
    END_REFLECTION();
}

int ServiceCounter::FindFreePlace()
{
    int indexFound = -1;
    const int placedDishesCount = placedDishes.size();
    for (int i = 0; i < placedDishesCount; i++)
    {
        if (!placedDishes[i].placedDish.lock())
        {
            indexFound = i;
            break;
        }
    }
    return indexFound;
}

int ServiceCounter::FindCleanDish()
{
    int indexFound = -1;
    const int placedDishesCount = placedDishes.size();
    for (int i = 0; i < placedDishesCount; i++)
    {
        if (placedDishes[i].placedDish.lock() && !placedDishes[i].isDirty)
        {
            indexFound = i;
            break;
        }
    }
    return indexFound;
}

void ServiceCounter::PlaceTake(std::shared_ptr<Player> player)
{
    std::shared_ptr<GameObject> heldItem = player->GetHeldItem();
    if (heldItem)
    {
        std::shared_ptr<Dish> dish = heldItem->GetComponent<Dish>();
        // Place a dish
        if (dish && dish->isFinished)
        {
            int index = FindFreePlace();
            if (index != -1)
            {
                heldItem->SetParent(dishPositions[index].lock());
                heldItem->GetTransform()->SetLocalPosition(Vector3(0));
                heldItem->GetTransform()->SetLocalEulerAngles(Vector3(0));
                placedDishes[index].placedDish = heldItem;
                player->RemoveHeldItem();
                placedDishes[index].removeTimer = 2;
                placedDishes[index].isDirty = true;
                bellAudioSource.lock()->Play();
            }
        }
    }
    else
    {
        // Take a dish
        int index = FindCleanDish();
        if (index != -1)
        {
            player->SetHeldItem(placedDishes[index].placedDish.lock());
            placedDishes[index].placedDish.reset();
        }
    }
}

void ServiceCounter::OnTriggerEnter(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(std::dynamic_pointer_cast<Interactable>(shared_from_this()));
}

void ServiceCounter::OnTriggerExit(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(nullptr);
}