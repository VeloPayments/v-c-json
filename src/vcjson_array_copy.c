/**
 * \file vcjson_array_copy.c
 *
 * \brief Copy an array instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_rbtree;
RCPR_IMPORT_resource;

/**
 * \brief Make a deep copy of the given \ref vcjson_array.
 *
 * \note On success, this function creates a  \ref vcjson_array instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param arr           Pointer to the array pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original array to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_array_copy(
    vcjson_array** arr, RCPR_SYM(allocator)* alloc, const vcjson_array* orig)
{
    status retval, release_retval;
    vcjson_array* tmp;
    vcjson_value* value;

    /* create a new array that is the same size as the original array. */
    retval = vcjson_array_create(&tmp, alloc, orig->elems);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* iterate through all array members. */
    for (size_t i = 0; i < orig->elems; ++i)
    {
        /* duplicate the value of this array member. */
        retval = vcjson_value_copy(&value, alloc, orig->arr[i]);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_tmp;
        }

        /* set this value in the new array. */
        retval = vcjson_array_set(tmp, i, value);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_value;
        }
    }

    /* Success. Recursive deep copy complete. */
    *arr = tmp;
    retval = STATUS_SUCCESS;
    goto done;

cleanup_value:
    release_retval = resource_release(&value->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
