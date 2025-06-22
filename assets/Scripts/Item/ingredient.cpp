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
    ADD_VARIABLE(height, true);
    ADD_VARIABLE(ingredientType, true);
    ADD_VARIABLE(cuttable, true);
    END_REFLECTION();
}
