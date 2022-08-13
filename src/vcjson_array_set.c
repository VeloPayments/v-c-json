/**
 * \file vcjson_array_set.c
 *
 * \brief Set an array element to the given value.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_resource;

/**
 * \brief Set the value of the \ref vcjson_array instance at the given offset.
 *
 * \note On success, this array instance takes ownership of the given \ref
 * vcjson_value instance. If there is already a value set by the array at the
 * given offset, it is reclaimed using \ref resource_release.
 *
 * \param arr           The array instance for this operation.
 * \param offset        The offset at which this value should be set.
 * \param value         The value to set.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_array_set(vcjson_array* arr, size_t offset, vcjson_value* value)
{
    status retval;

    /* array bounds check. */
    if (offset >= arr->elems)
    {
        return ERROR_VCJSON_ARRAY_INDEX_OUT_OF_BOUNDS;
    }

    /* release the previous value. */
    retval = resource_release(&(arr->arr[offset]->hdr));
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* set this array element to value. */
    arr->arr[offset] = value;
    return STATUS_SUCCESS;
}
