#pragma once


#include <engine/component.h>
#include "../interactable.h"

class BoxCollider;
class GameManager;
class AudioSource;

class DishSlot
{
public:
    std::weak_ptr<GameObject> placedDish;
    bool isDirty;
    float removeTimer;
};

class ServiceCounter : public Component, public Interactable
{
public:
    ServiceCounter();

    void Start() override;
    void Update() override;

    ReflectiveData GetReflectiveData() override;

    void PlaceTake(std::shared_ptr<Player> player) override;

    void OnTriggerEnter(CollisionEvent eventData) override;
    void OnTriggerExit(CollisionEvent eventData) override;

    int FindFreePlace();
    int FindCleanDish();

    std::weak_ptr<GameManager> gameManager;
    std::vector<std::weak_ptr<BoxCollider>> triggers;
    std::vector<std::weak_ptr<GameObject>> dishPositions;
    std::vector<DishSlot> placedDishes;
    std::weak_ptr<AudioSource> bellAudioSource;
    std::weak_ptr<AudioSource> popAudioSource;
    // std::vector<std::weak_ptr<GameObject>> placedDishes;
    // std::vector<std::weak_ptr<GameObject>> placedDishesRemoveTimer;
    float timer = 5;
    int dishCountToReturn = 0;

private:
};
