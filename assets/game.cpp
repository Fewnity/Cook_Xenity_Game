#include "game.h"

#include <engine/class_registry/class_registry.h>

//----- Include all your classes here:
#include "Scripts/Player/player.h"
#include "Scripts/Furniture/stove.h"
#include "Scripts/Furniture/counter.h"
#include "Scripts/Furniture/crate.h"
#include "Scripts/Furniture/sink.h"
#include "Scripts/Furniture/dish_rack.h"
#include "Scripts/game_manager.h"
#include "Scripts/Item/dish.h"
#include "Scripts/Item/item_builder.h"
#include "Scripts/Item/ingredient.h"
#include "Scripts/Furniture/bin.h"
#include "Scripts/Furniture/service_counter.h"
#include "Scripts/loading_bar.h"
#include "Scripts/Furniture/stove_audio_source.h"
#include "Scripts/auto_destroy.h"

/**
* Register all your classes here:
*/
void Game::Start()
{
	REGISTER_COMPONENT(GameManager).DisableUpdateFunction();
	REGISTER_COMPONENT(Player);
	REGISTER_COMPONENT(Counter);
	REGISTER_COMPONENT(Stove);
	REGISTER_COMPONENT(Crate).DisableUpdateFunction();
	REGISTER_COMPONENT(Sink);
	REGISTER_COMPONENT(DishRack).DisableUpdateFunction();
	REGISTER_COMPONENT(Dish).DisableUpdateFunction();
	REGISTER_COMPONENT(ItemBuilder).DisableUpdateFunction();
	REGISTER_COMPONENT(Ingredient).DisableUpdateFunction();
	REGISTER_COMPONENT(Bin).DisableUpdateFunction();
	REGISTER_COMPONENT(ServiceCounter);
	REGISTER_COMPONENT(LoadingBar).DisableUpdateFunction();
	REGISTER_COMPONENT(StoveAudioSource).DisableUpdateFunction();
	REGISTER_COMPONENT(AutoDestroy);
}