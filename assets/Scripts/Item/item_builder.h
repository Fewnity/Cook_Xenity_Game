#pragma once


#include <engine/component.h>
#include "meal_type.h"
#include "ingredient_type.h"

class Recipe
{
public:
    IngredientType startingIngredient;
    std::vector<IngredientType> neededIngredients;
};

class ItemBuilder : public Component
{
public:
    ItemBuilder();

    void Start() override;

    ReflectiveData GetReflectiveData() override;

    std::shared_ptr<GameObject> CreateIngredient(IngredientType type);
    std::shared_ptr<GameObject> CreateDish(bool dirty);

    std::weak_ptr<GameObject> tomatoSlices;
    std::weak_ptr<GameObject> cheeseSlice;
    std::weak_ptr<GameObject> cookedSteak;
    std::weak_ptr<GameObject> overcookedSteak;
    std::weak_ptr<GameObject> dish;
    static std::map<MealType, Recipe> Recipes;

private:
};
