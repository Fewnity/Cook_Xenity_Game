#pragma once

#include <engine/component.h>
#include "../interactable.h"

class BoxCollider;
class GameManager;

class Bin : public Component, public Interactable
{
public:
    Bin();

    void Start() override;
    void Update() override;

    ReflectiveData GetReflectiveData() override;

    void PlaceTake(std::shared_ptr<Player> player) override;
    
    void OnTriggerEnter(CollisionEvent eventData) override;
    void OnTriggerExit(CollisionEvent eventData) override;

    std::vector<std::weak_ptr<BoxCollider>> triggers;
    std::weak_ptr<GameManager> gameManager;

private:
};
