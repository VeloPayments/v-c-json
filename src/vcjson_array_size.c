/**
 * \file vcjson_array_size.c
 *
 * \brief Get the number of array elements.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the size of the given array instance.
 *
 * \param arr           The array instance for this operation.
 *
 * \returns the size of this array instance.
 */
size_t vcjson_array_size(const vcjson_array* arr)
{
    return arr->elems;
}
