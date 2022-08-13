/**
 * \file vcjson_array_create.c
 *
 * \brief Create an array instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/**
 * \brief Create a \ref vcjson_array using the given allocator and size.
 *
 * \note On success, this function creates a  \ref vcjson_array instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle. By default, all values in this array will be initialized to \ref
 * VCJSON_NULL. The caller should use \ref vcjson_array_set and \ref
 * vcjson_array_get to change and get values in this array, respectfully.
 *
 * \param arr           Pointer to the array pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param size          The number of elements in this array.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_array_create(
    vcjson_array** arr, RCPR_SYM(allocator)* alloc, size_t size)
{
    status retval, release_retval;
    vcjson_array* tmp;

    /* attempt to allocate memory for this array instance. */
    retval = allocator_allocate(alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear instance. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize resource. */
    resource_init(&tmp->hdr, &vcjson_array_resource_release);

    /* set initial values. */
    tmp->alloc = alloc;
    tmp->elems = size;

    /* get the null value. */
    vcjson_value* nullval;
    retval = vcjson_value_create_from_null(&nullval, alloc);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* attempt to allocate memory for the array elements. */
    retval =
        allocator_allocate(
            alloc, (void**)&tmp->arr, size * sizeof(vcjson_value*));
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* set each array element to NULL. */
    for (size_t i = 0; i < size; ++i)
    {
        tmp->arr[i] = nullval;
    }

    /* success. */
    *arr = tmp;
    retval = STATUS_SUCCESS;
    goto done;

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
