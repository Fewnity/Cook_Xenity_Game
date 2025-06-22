#pragma once


#include <engine/component.h>
#include "../interactable.h"

class BoxCollider;
class GameManager;

class DishRack : public Component, public Interactable
{
public:
    DishRack();

    void Start() override;

    ReflectiveData GetReflectiveData() override;

    void PlaceTake(std::shared_ptr<Player> player) override;
    void UpdateGraphics();

    void OnTriggerEnter(CollisionEvent eventData) override;
    void OnTriggerExit(CollisionEvent eventData) override;
    
    std::vector<std::weak_ptr<BoxCollider>> triggers;
    std::weak_ptr<GameManager> gameManager;

    std::vector<std::weak_ptr<GameObject>> dishPlates;

    std::weak_ptr<GameObject> dishPlate;
    int dishCount = 4;

private:
    int currentDishCount = 4;
};
