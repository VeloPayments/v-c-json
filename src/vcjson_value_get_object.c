/**
 * \file vcjson_value_get_object.c
 *
 * \brief Attempt to coerce an object from a value.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Attempt to get the \ref vcjson_object value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the object pointer to the object value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param object        Pointer to the object pointer to hold the object value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not an object.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_object(vcjson_object** object, vcjson_value* value)
{
    /* verify that this value is the right type. */
    if (VCJSON_VALUE_TYPE_OBJECT != value->type)
    {
        return ERROR_VCJSON_INVALID_GET;
    }

    /* return the value. */
    *object = (vcjson_object*)value->value;
    return STATUS_SUCCESS;
}
