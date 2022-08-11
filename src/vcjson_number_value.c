/**
 * \file vcjson_number_value.c
 *
 * \brief Get the number value of a \ref vcjson_number instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the number value of this \ref vcjson_number instance.
 *
 * \param number        The number instance for this operation.
 *
 * \returns the number value for this instance.
 */
double vcjson_number_value(const vcjson_number* number)
{
    return number->value;
}
