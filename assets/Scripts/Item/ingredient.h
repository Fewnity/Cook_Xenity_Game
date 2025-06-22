#pragma once


#include <engine/component.h>

#include "ingredient_type.h"

class Ingredient : public Component
{
public:
    Ingredient();

    ReflectiveData GetReflectiveData() override;
    float height = 0;
    bool cuttable = false;
    IngredientType ingredientType = IngredientType::None;

private:
};
