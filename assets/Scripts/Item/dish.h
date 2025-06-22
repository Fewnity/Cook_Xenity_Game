#pragma once


#include <engine/component.h>
#include "ingredient_type.h"
#include "meal_type.h"

class Ingredient;
class ParticleSystem;
class AudioSource;

class Dish : public Component
{
public:
    Dish();

    ReflectiveData GetReflectiveData() override;

    std::vector<std::weak_ptr<Ingredient>> ingredients;

    bool AddIngredient(std::weak_ptr<Ingredient> ingredient);
    bool CanAddIngredient(std::shared_ptr<Ingredient> ingredientToAdd);
    void UpdateGraphics();
    bool IsIngredientNeeded(IngredientType type);
    bool CheckIsFinished();

    MealType mealType = MealType::None;

    std::vector<std::weak_ptr<GameObject>> buns;

    std::weak_ptr<GameObject> cleanPlate;
    std::weak_ptr<GameObject> dirtyPlate;
    std::weak_ptr<ParticleSystem> particleSystem;
    std::weak_ptr<AudioSource> audioSource;

    bool isDirty = false;
    bool isFinished = false;
    void RemoveIngredients();

private:
};
