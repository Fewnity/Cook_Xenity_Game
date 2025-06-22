#pragma once


#include <engine/component.h>
#include "../interactable.h"

class BoxCollider;
class GameManager;
class LoadingBar;
class AudioSource;
class ParticleSystem;

class Sink : public Component, public Interactable
{
public:
    Sink();

    void Start() override;
    void Update() override;

    ReflectiveData GetReflectiveData() override;

    void Interact(std::shared_ptr<Player> player) override;
    void StopInteract() override;
    void PlaceTake(std::shared_ptr<Player> player) override;

    void OnTriggerEnter(CollisionEvent eventData) override;
    void OnTriggerExit(CollisionEvent eventData) override;
    
    std::vector<std::weak_ptr<BoxCollider>> triggers;
    std::weak_ptr<GameManager> gameManager;
    std::weak_ptr<GameObject> dishPosition;
    std::weak_ptr<GameObject> placedDish;
    std::weak_ptr<LoadingBar> loadingBar;
    float washTime = 3;
    std::weak_ptr<AudioSource> audioSource;
    std::weak_ptr<ParticleSystem> particleSystem;

private:
    float useTimer = 3;
};
