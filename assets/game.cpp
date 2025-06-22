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
	REGISTER_COMPONENT(GameManager, true); // true = disable update function
	REGISTER_COMPONENT(Player, false); // false = enable update function
	REGISTER_COMPONENT(Counter, false);
	REGISTER_COMPONENT(Stove, false);
	REGISTER_COMPONENT(Crate, true);
	REGISTER_COMPONENT(Sink, false);
	REGISTER_COMPONENT(DishRack, true);
	REGISTER_COMPONENT(Dish, true);
	REGISTER_COMPONENT(ItemBuilder, true);
	REGISTER_COMPONENT(Ingredient, true);
	REGISTER_COMPONENT(Bin, true);
	REGISTER_COMPONENT(ServiceCounter, false);
	REGISTER_COMPONENT(LoadingBar, true);
	REGISTER_COMPONENT(StoveAudioSource, true);
	REGISTER_COMPONENT(AutoDestroy, false);
}