#include "item_builder.h"
#include <xenity.h>
#include "dish.h"

std::map<MealType, Recipe> ItemBuilder::Recipes;

/**
 * Constructor
 */
ItemBuilder::ItemBuilder()
{
}

/**
 * Called once when the script is enabled
 */
void ItemBuilder::Start()
{
    Recipe burgerRecipe;
    burgerRecipe.startingIngredient = IngredientType::Buns;
    burgerRecipe.neededIngredients.push_back(IngredientType::TomatoSlice);
    burgerRecipe.neededIngredients.push_back(IngredientType::CheeseSlice);
    burgerRecipe.neededIngredients.push_back(IngredientType::CookedSteak);

    Recipes[MealType::Burger] = burgerRecipe;
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData ItemBuilder::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(tomatoSlices);
    ADD_VARIABLE(cheeseSlice);
    ADD_VARIABLE(cookedSteak);
    ADD_VARIABLE(overcookedSteak);
    ADD_VARIABLE(dish);
    END_REFLECTION();
}

std::shared_ptr<GameObject> ItemBuilder::CreateIngredient(IngredientType type)
{
    std::shared_ptr<GameObject> go;
    if (type == IngredientType::TomatoSlice)
        go = Instantiate(tomatoSlices.lock());
    else if (type == IngredientType::CheeseSlice)
        go = Instantiate(cheeseSlice.lock());
    else if (type == IngredientType::CookedSteak)
        go = Instantiate(cookedSteak.lock());
    else if (type == IngredientType::OvercookedSteak)
        go = Instantiate(overcookedSteak.lock());

    return go;
}

std::shared_ptr<GameObject> ItemBuilder::CreateDish(bool dirty)
{
    std::shared_ptr<GameObject> go;
    go = Instantiate(dish.lock());
    if (std::shared_ptr<Dish> dish = go->GetComponent<Dish>())
    {
        dish->isDirty = dirty;
        dish->UpdateGraphics();
    }
    return go;
}
