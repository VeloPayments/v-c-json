/**
 * \file vcjson_value_get_number.c
 *
 * \brief Attempt to coerce a number from this value.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Attempt to get the \ref vcjson_number value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the number pointer to the number value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param number        Pointer to the number pointer to hold the number value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not a number.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_number(vcjson_number** number, vcjson_value* value)
{
    /* verify that this is a number type. */
    if (VCJSON_VALUE_TYPE_NUMBER != value->type)
    {
        return ERROR_VCJSON_INVALID_GET;
    }

    /* return the number value. */
    *number = (vcjson_number*)value->value;
    return STATUS_SUCCESS;
}
