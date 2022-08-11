/**
 * \file vcjson_string_value.c
 *
 * \brief Get the string value of a \ref vcjson_string instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the string value of this \ref vcjson_string instance.
 *
 * \note The string value returned by this accessor is owned by the
 * \ref vcjson_string instance and its lifetime is the same as the lifetime of
 * that instance.
 *
 * \param string        The string instance for this operation.
 *
 * \returns the string value for this instance.
 */
const char* vcjson_string_value(const vcjson_string* string)
{
    return string->value;
}
