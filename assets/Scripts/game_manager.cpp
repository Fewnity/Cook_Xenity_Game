#include "game_manager.h"
#include <xenity.h>

#include "Player/player.h"
#include "Item/item_builder.h"

/**
 * Constructor
 */
GameManager::GameManager()
{
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData GameManager::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(players, true);
    ADD_VARIABLE(itemBuilder, true);
    END_REFLECTION();
}
