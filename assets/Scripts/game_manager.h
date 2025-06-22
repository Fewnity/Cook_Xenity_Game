#pragma once


#include <engine/component.h>

class Player;
class ItemBuilder;

class GameManager : public Component
{
public:
    GameManager();
    
    ReflectiveData GetReflectiveData() override;
    std::vector<std::weak_ptr<Player>> players;
    std::weak_ptr<ItemBuilder> itemBuilder;

private:
};
