#pragma once

#include <engine/component.h>
#include "../interactable.h"

class BoxCollider;
class GameManager;
class LoadingBar;

class Counter : public Component, public Interactable
{
public:
    Counter();

    void Start() override;
    void Update() override;

    void PlaceTake(std::shared_ptr<Player> player) override;
    void Interact(std::shared_ptr<Player> player) override;
    void StopInteract() override;

     void OnTriggerEnter(CollisionEvent eventData) override;
    void OnTriggerExit(CollisionEvent eventData) override;
    
    std::vector<std::weak_ptr<BoxCollider>> triggers;

    ReflectiveData GetReflectiveData() override;
    std::weak_ptr<GameManager> gameManager;
    std::weak_ptr<GameObject> middlePosition;
    std::weak_ptr<GameObject> itemOnTop;
    std::weak_ptr<LoadingBar> loadingBar;
    float cutTime = 3;

private:
    float useTimer = 3;
};
