#pragma once


#include <engine/component.h>
#include <engine/math/vector3.h>

class RigidBody;
class Interactable;

class Player : public Component
{
public:
    Player();

    void Start() override;
    void Update() override;

    void SetHeldItem(std::shared_ptr<GameObject> item);
    std::shared_ptr<GameObject> GetHeldItem();
    void RemoveHeldItem();

    ReflectiveData GetReflectiveData() override;
    void SetInteractable(std::shared_ptr<Interactable> newInteractable);
    std::shared_ptr<Interactable> GetInteractable();

private:
    float moveSpeed = 2;
    std::weak_ptr<RigidBody> rigidbody;
    std::weak_ptr<Interactable> interactable;
    std::weak_ptr<GameObject> handsLocation;
    std::weak_ptr<GameObject> heldItem;

    std::weak_ptr<GameObject> playerMeshHolding;
    std::weak_ptr<GameObject> playerMeshIdle;
};
