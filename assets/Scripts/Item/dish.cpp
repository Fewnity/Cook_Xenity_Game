#include "dish.h"
#include <xenity.h>
#include "ingredient.h"
#include "item_builder.h"

/**
 * Constructor
 */
Dish::Dish()
{
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData Dish::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(buns, true);
    ADD_VARIABLE(cleanPlate, true);
    ADD_VARIABLE(dirtyPlate, true);
    ADD_VARIABLE(particleSystem, true);
    ADD_VARIABLE(audioSource, true);
    END_REFLECTION();
}

bool Dish::AddIngredient(std::weak_ptr<Ingredient> weakIngredient)
{
    if (isDirty || isFinished)
        return false;

    bool added = false;
    if (std::shared_ptr<Ingredient> ingredient = weakIngredient.lock())
    {
        bool canAdd = CanAddIngredient(ingredient);
        if (canAdd)
        {
            IngredientType ingredientType = ingredient->ingredientType;
            
            if (mealType == MealType::None) // If the dish is empty, create a meal based on the first added ingredient
            {
                if (ingredientType == IngredientType::Buns)
                {
                    mealType = MealType::Burger;
                    added = true;
                    ingredients.push_back(ingredient);
                    ingredient->GetGameObject()->SetActive(false);
                    ingredient->GetGameObject()->SetParent(GetGameObject());
                }
            }
            else
            {
                bool isForMeal = IsIngredientNeeded(ingredientType);
                if (isForMeal)
                {
                    ingredients.push_back(ingredient);
                    ingredient->GetGameObject()->SetParent(GetGameObject());
                    added = true;
                }
            }
        }
    }

    // If the ingredient has been added, update visuals and sounds
    if (added)
    {
        UpdateGraphics();
        if (CheckIsFinished())
        {
            isFinished = true;
            particleSystem.lock()->Play();
            audioSource.lock()->Play();
        }
    }

    return added;
}

bool Dish::CanAddIngredient(std::shared_ptr<Ingredient> ingredientToAdd)
{
    if (!ingredientToAdd)
        return false;

    // Check if the ingredient to add is already in the dish
    bool alreadyAdded = false;
    IngredientType type = ingredientToAdd->ingredientType;
    for (std::weak_ptr<Ingredient> weakIngredient : ingredients)
    {
        std::shared_ptr<Ingredient> ingredient = weakIngredient.lock();
        if (ingredient && ingredient->ingredientType == type)
        {
            alreadyAdded = true;
            break;
        }
    }

    if (alreadyAdded)
        return false;

    return true;
}

void Dish::UpdateGraphics()
{
    float currentHeight = 0;
    if (isDirty)
    {
        cleanPlate.lock()->SetActive(false);
        dirtyPlate.lock()->SetActive(true);
    }
    else
    {
        cleanPlate.lock()->SetActive(true);
        dirtyPlate.lock()->SetActive(false);

        if (mealType == MealType::Burger)
        {
            buns[0].lock()->SetActive(true);
            buns[1].lock()->SetActive(true);
            currentHeight += buns[0].lock()->GetComponent<Ingredient>()->height + buns[0].lock()->GetTransform()->GetLocalPosition().y;
        }
        else
        {
            buns[0].lock()->SetActive(false);
            buns[1].lock()->SetActive(false);
        }

        // Make a pile of ingredients
        for (std::weak_ptr<Ingredient> weakIngredient : ingredients)
        {
            std::shared_ptr<Ingredient> ingredient = weakIngredient.lock();
            if (ingredient)
            {
                ingredient->GetTransform()->SetLocalPosition(Vector3(0, currentHeight, 0));
                ingredient->GetTransform()->SetLocalEulerAngles(Vector3(0, 0, 0));
                currentHeight += ingredient->height;
            }
        }

        if (mealType == MealType::Burger)
        {
            buns[1].lock()->GetTransform()->SetLocalPosition(Vector3(0, currentHeight, 0));
        }
    }
}

bool Dish::IsIngredientNeeded(IngredientType type)
{
    Recipe &recipe = ItemBuilder::Recipes[mealType];
    for (IngredientType typeToCheck : recipe.neededIngredients)
    {
        if (typeToCheck == type)
        {
            return true;
        }
    }
    return false;
}

bool Dish::CheckIsFinished()
{
    Recipe &recipe = ItemBuilder::Recipes[mealType];
    const size_t recipeIngredientCount = recipe.neededIngredients.size() + 1;
    if (recipeIngredientCount == ingredients.size())
    {
        return true;
    }

    return false;
}

void Dish::RemoveIngredients()
{
    if (ingredients.size() == 0)
        return;

    isDirty = true;
    isFinished = false;
    mealType = MealType::None;
    for (std::weak_ptr<Ingredient> weakIngredient : ingredients)
    {
        std::shared_ptr<Ingredient> ingredient = weakIngredient.lock();
        if (ingredient)
        {
            Destroy(ingredient->GetGameObject());
        }
    }
    ingredients.clear();
    UpdateGraphics();
}
