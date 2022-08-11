/**
 * \file vcjson_string_create.c
 *
 * \brief Create a string instance.
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
 * string value.
 *
 * \note On success, this function creates a \ref vcjson_string instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param string        Pointer to the string pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param value         The string value to be used for this instance. This
 *                      value is copied; the original value is assumed owned by
 *                      the caller.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_string_create(
    vcjson_string** string, RCPR_SYM(allocator)* alloc, const char* value)
{
    status retval, release_retval;
    vcjson_string* tmp;

    /* get the length of the string. */
    size_t length = strlen(value);

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
    tmp->length = length;

    /* allocate memory for string. */
    retval = allocator_allocate(alloc, (void**)&tmp->value, length + 1);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* copy string. */
    memcpy(tmp->value, value, length);
    tmp->value[length] = 0;

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
