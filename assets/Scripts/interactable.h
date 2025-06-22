#pragma once


#include <engine/component.h>

ENUM(TriggerState, NoTrigger, Enter, In, Waiting);

class Player;

class Interactable
{
public:
    Interactable();

    virtual void Interact(std::shared_ptr<Player> player) {}
    virtual void StopInteract() {}
    virtual void PlaceTake(std::shared_ptr<Player> player) {}

protected:
    bool isUsed = false;
    std::weak_ptr<Player> usedBy;
    TriggerState triggerState = TriggerState::NoTrigger;
};
