#include "player.h"
#include <xenity.h>
#include "../interactable.h"
#include <engine/math/math.h>
#include <engine/application.h>
#include <math.h>

/**
 * Constructor
 */
Player::Player()
{
}

/**
 * Called once when the script is enabled
 */
void Player::Start()
{
}

/**
 * Called every frame
 */
void Player::Update()
{
    if (!rigidbody.lock())
    {
        Debug::PrintError("[Player::Update] Missing rigidbody!");
        return;
    }

    std::shared_ptr<Transform> transform = GetTransform();
    Vector3 moveDir;

    if (InputSystem::GetKey(KeyCode::RIGHT))
    {
        moveDir.x = 1;
    }
    else if (InputSystem::GetKey(KeyCode::LEFT))
    {
        moveDir.x = -1;
    }
    if (InputSystem::GetKey(KeyCode::UP))
    {
        moveDir.z = 1;
    }
    else if (InputSystem::GetKey(KeyCode::DOWN))
    {
        moveDir.z = -1;
    }
    if (moveDir.x == 0 && moveDir.z == 0)
    {
        moveDir.x = InputSystem::GetLeftJoystick(0).x;
        moveDir.z = -InputSystem::GetLeftJoystick(0).y;
    }

    if (InputSystem::GetKeyDown(KeyCode::CROSS) || InputSystem::GetKeyDown(KeyCode::F))
    {
        if (interactable.lock())
        {
            interactable.lock()->PlaceTake(std::dynamic_pointer_cast<Player>(shared_from_this()));
        }
    }
    if (InputSystem::GetKeyDown(KeyCode::SQUARE)|| InputSystem::GetKeyDown(KeyCode::E))
    {
        if (interactable.lock())
        {
            interactable.lock()->Interact(std::dynamic_pointer_cast<Player>(shared_from_this()));
        }
    }
    if (InputSystem::GetKeyUp(KeyCode::SQUARE)|| InputSystem::GetKeyUp(KeyCode::E))
    {
        if (interactable.lock())
        {
            interactable.lock()->StopInteract();
        }
    }

    if (rigidbody.lock())
    {
        rigidbody.lock()->SetVelocity(moveDir * moveSpeed);
    }    

    if (moveDir.x != 0 || moveDir.z != 0)
    {
        Vector3 angle;
        angle.y = atan2f(moveDir.x, moveDir.z) * 180 / Math::PI;
        transform->SetEulerAngles(angle);
    }
}

void Player::SetHeldItem(std::shared_ptr<GameObject> item)
{
    if (!handsLocation.lock())
    {
        Debug::PrintError("[Player::SetHeldItem] handsLocation is not filled!");
        return;
    }

    item->SetParent(handsLocation.lock());
    item->GetTransform()->SetLocalPosition(Vector3(0));
    item->GetTransform()->SetLocalEulerAngles(Vector3(0));
    heldItem = item;
    playerMeshHolding.lock()->SetActive(true);
    playerMeshIdle.lock()->SetActive(false);
}

std::shared_ptr<GameObject> Player::GetHeldItem()
{
    return heldItem.lock();
}

void Player::RemoveHeldItem()
{
    heldItem.reset();
    playerMeshHolding.lock()->SetActive(false);
    playerMeshIdle.lock()->SetActive(true);
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData Player::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(moveSpeed);
    ADD_VARIABLE(rigidbody);
    ADD_VARIABLE(handsLocation);
    ADD_VARIABLE(heldItem);
    ADD_VARIABLE(playerMeshHolding);
    ADD_VARIABLE(playerMeshIdle);
    END_REFLECTION();
}

void Player::SetInteractable(std::shared_ptr<Interactable> newInteractable)
{
    if (interactable.lock() && !newInteractable)
    {
        interactable.lock()->StopInteract();
    }
    interactable = newInteractable;
}

std::shared_ptr<Interactable> Player::GetInteractable()
{
    return interactable.lock();
}
