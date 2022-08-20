/**
 * \file vcjson_string_create_from_raw.c
 *
 * \brief Create a string instance from a raw string value.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/**
 * \brief Create a \ref vcjson_string instance using the given allocator and
 * raw string value.
 *
 * \note On success, this function creates a \ref vcjson_string instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param string        Pointer to the string pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param value         The raw string value to be used for this instance. This
 *                      value is copied; the original value is assumed owned by
 *                      the caller.
 * \param size          The size of this raw string value.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_string_create_from_raw(
    vcjson_string** string, RCPR_SYM(allocator)* alloc, const char* value,
    size_t size)
{
    status retval, release_retval;
    vcjson_string* tmp;

    /* allocate memory for the string instance. */
    retval = allocator_allocate(alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear instance. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize resource. */
    resource_init(&tmp->hdr, &vcjson_string_resource_release);

    /* set initial values. */
    tmp->alloc = alloc;
    tmp->length = size;

    /* allocate memory for string. */
    retval = allocator_allocate(alloc, (void**)&tmp->value, size);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* copy string. */
    memcpy(tmp->value, value, size);

    /* success. */
    *string = tmp;
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
