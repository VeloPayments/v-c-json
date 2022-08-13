/**
 * \file vcjson_array_get.c
 *
 * \brief Get an array element at the given index.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the value of the \ref vcjson_array instance at the given offset.
 *
 * \note On success, the value at the given offset of the array is returned.
 * This value remains owned by this array instance.
 *
 * \param value         Pointer to the value pointer to receive this value. Note
 *                      that this value is owned by the array instance.
 * \param arr           The array instance for this operation.
 * \param offset        The offset for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_array_get(vcjson_value** value, vcjson_array* arr, size_t offset)
{
    /* array bounds check. */
    if (offset >= arr->elems)
    {
        return ERROR_VCJSON_ARRAY_INDEX_OUT_OF_BOUNDS;
    }

    /* return the value at the given offset. */
    *value = arr->arr[offset];
    return STATUS_SUCCESS;
}
