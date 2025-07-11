#include "ingredient.h"
#include <xenity.h>

/**
 * Constructor
 */
Ingredient::Ingredient()
{
}

/**
 * Lists all variables to show in inspector
 */
ReflectiveData Ingredient::GetReflectiveData()
{
    BEGIN_REFLECTION();
    ADD_VARIABLE(height);
    ADD_VARIABLE(ingredientType);
    ADD_VARIABLE(cuttable);
    END_REFLECTION();
}
