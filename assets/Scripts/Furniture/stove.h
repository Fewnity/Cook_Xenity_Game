#pragma once


#include <engine/component.h>
#include "../interactable.h"

ENUM(CookingState, NotCooked, Cooked, Overcooked);

class BoxCollider;
class GameManager;
class Ingredient;
class LoadingBar;
class StoveAudioSource;
class ParticleSystem;

class Stove : public Component, public Interactable
{
public:
    Stove();

    void Start() override;
    void Update() override;

    void PlaceTake(std::shared_ptr<Player> player) override;

    void OnTriggerEnter(CollisionEvent eventData) override;
    void OnTriggerExit(CollisionEvent eventData) override;

    std::vector<std::weak_ptr<BoxCollider>> triggers;

    ReflectiveData GetReflectiveData() override;
    std::weak_ptr<GameManager> gameManager;
    std::weak_ptr<Ingredient> cookingIngredient;
    std::weak_ptr<GameObject> ingredientPosition;
    CookingState cookingState = CookingState::NotCooked;

    std::weak_ptr<LoadingBar> loadingBar;
    float cookTime = 10;
    std::weak_ptr<StoveAudioSource> stoveAudioSource;
    std::weak_ptr<ParticleSystem> particleSystem;

private:
    float timer = 0;
};
