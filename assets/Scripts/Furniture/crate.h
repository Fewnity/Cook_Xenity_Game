#pragma once


#include <engine/component.h>
#include "../interactable.h"
#include "../Item/ingredient.h"

class BoxCollider;
class GameManager;
class AudioSource;

class Crate : public Component, public Interactable
{
public:
    Crate();

    void Start() override;

    ReflectiveData GetReflectiveData() override;

    void PlaceTake(std::shared_ptr<Player> player) override;

    void OnTriggerEnter(CollisionEvent eventData) override;
    void OnTriggerExit(CollisionEvent eventData) override;

    std::vector<std::weak_ptr<BoxCollider>> triggers;
    std::weak_ptr<GameManager> gameManager;
    std::weak_ptr<GameObject> ingredientToSpawn;
    IngredientType ingredientType = IngredientType::None;
    std::weak_ptr<AudioSource> audioSource;

private:
};
