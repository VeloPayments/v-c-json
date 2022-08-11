/**
 * \file vcjson_value_type.c
 *
 * \brief Get the type of the \ref vcjson_value instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the value type of this \ref vcjson_value instance.
 *
 * \param value         The value instance for this operation.
 *
 * \returns the value type for this instance.
 */
int vcjson_value_type(const vcjson_value* value)
{
    return value->type;
}
