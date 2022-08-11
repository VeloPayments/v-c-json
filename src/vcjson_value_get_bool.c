/**
 * \file vcjson_value_get_bool.c
 *
 * \brief Attempt to coerce a value to boolean.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Attempt to get the \ref vcjson_bool value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the bool pointer to the bool value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param boolean       Pointer to the bool pointer to hold the bool value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not a bool.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_bool(vcjson_bool** boolean, vcjson_value* value)
{
    /* verify that this value is of the right type. */
    if (VCJSON_VALUE_TYPE_BOOL != value->type)
    {
        return ERROR_VCJSON_INVALID_GET;
    }

    /* return the value. */
    *boolean = (vcjson_bool*)value->value;
    return STATUS_SUCCESS;
}
