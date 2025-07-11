#include "sink.h"
#include <xenity.h>
#include "../game_manager.h"
#include "../Player/player.h"
#include "../Item/dish.h"
#include "../loading_bar.h"

/**
 * Constructor
 */
Sink::Sink()
{
}

/**
 * Called once when the script is enabled
 */
void Sink::Start()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[Sink::Start] gameManager is not filled!");
        return;
    }

    particleSystem.lock()->SetIsEmitting(false);
}

/**
 * Called every frame
 */
void Sink::Update()
{
    if (!gameManager.lock())
    {
        Debug::PrintError("[Sink::Update] gameManager is not filled!");
        return;
    }

    // If the player uses the sink, wait and make the dish clean at the end of the timer
    if (isUsed)
    {
        useTimer -= Time::GetDeltaTime();
        loadingBar.lock()->SetFillAmount(1 - (useTimer / washTime));

        if (useTimer <= 0)
        {
            isUsed = false;
            audioSource.lock()->Stop();
            particleSystem.lock()->SetIsEmitting(false);
            loadingBar.lock()->GetGameObject()->SetActive(false);
            if (placedDish.lock())
            {
                std::shared_ptr<Dish> dish = placedDish.lock()->GetComponent<Dish>();
                if (dish && dish->isDirty)
                {
                    dish->isDirty = false;
                    dish->UpdateGraphics();
                }
            }
        }
    }
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData Sink::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(triggers);
    ADD_VARIABLE(gameManager);
    ADD_VARIABLE(dishPosition);
    ADD_VARIABLE(loadingBar);
    ADD_VARIABLE(washTime);
    ADD_VARIABLE(audioSource);
    ADD_VARIABLE(particleSystem);
    END_REFLECTION();
}

void Sink::Interact(std::shared_ptr<Player> player)
{
    if (placedDish.lock())
    {
        std::shared_ptr<Dish> dish = placedDish.lock()->GetComponent<Dish>();
        if (dish && dish->isDirty)
        {
            isUsed = true;
            audioSource.lock()->Play();
            particleSystem.lock()->SetIsEmitting(true);
            useTimer = washTime;
            loadingBar.lock()->GetGameObject()->SetActive(true);
            loadingBar.lock()->SetFillAmount(0);
        }
    }
}

void Sink::StopInteract()
{
    isUsed = false;
    loadingBar.lock()->GetGameObject()->SetActive(false);
    audioSource.lock()->Stop();
    particleSystem.lock()->SetIsEmitting(false);
}

void Sink::PlaceTake(std::shared_ptr<Player> player)
{
    std::shared_ptr<GameObject> heldItem = player->GetHeldItem();
    if (heldItem)
    {
        // Place a dish
        if (!placedDish.lock())
        {
            std::shared_ptr<Dish> dish = heldItem->GetComponent<Dish>();
            if (dish && dish->isDirty)
            {
                dish->GetGameObject()->SetParent(dishPosition.lock());
                dish->GetTransform()->SetLocalPosition(Vector3(0));
                dish->GetTransform()->SetLocalEulerAngles(Vector3(0));
                placedDish = dish->GetGameObject();
                player->RemoveHeldItem();
            }
        }
    }
    else if (placedDish.lock())
    {
        // Take a dish
        player->SetHeldItem(placedDish.lock());
        placedDish.reset();
    }
}

void Sink::OnTriggerEnter(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(std::dynamic_pointer_cast<Interactable>(shared_from_this()));
}

void Sink::OnTriggerExit(CollisionEvent eventData)
{
    gameManager.lock()->players[0].lock()->SetInteractable(nullptr);
}