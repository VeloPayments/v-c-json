/**
 * \file vcjson_value_get_array.c
 *
 * \brief Attempt to coerce an array from a value.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Attempt to get the \ref vcjson_array value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the array pointer to the array value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param arr           Pointer to the array pointer to hold the array value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not an array.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_array(vcjson_array** arr, vcjson_value* value)
{
    /* verify that this value is the right type. */
    if (VCJSON_VALUE_TYPE_ARRAY != value->type)
    {
        return ERROR_VCJSON_INVALID_GET;
    }

    /* return the value. */
    *arr = (vcjson_array*)value->value;
    return STATUS_SUCCESS;
}
