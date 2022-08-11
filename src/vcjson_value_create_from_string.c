/**
 * \file vcjson_value_create_from_string.c
 *
 * \brief Create a value wrapper for a string.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/**
 * \brief Create a \ref vcjson_value instance from a \ref vcjson_string
 * instance.
 *
 * \note On success, this function creates a \ref vcjson_value instance. The
 * \ref vcjson_string instance that it wraps is owned by this value instance.
 * This value instance is a resource that is owned by the caller. When no longer
 * needed, this resource must be released by calling \ref resource_release on
 * its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param string        The string instance that will be wrapped by this value
 *                      instance on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_string(
    vcjson_value** value, RCPR_SYM(allocator)* alloc, vcjson_string* string)
{
    status retval;
    vcjson_value* tmp;

    /* allocate memory for this value. */
    retval = allocator_allocate(alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear memory. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize resource. */
    resource_init(&tmp->hdr, &vcjson_value_with_resource_release);

    /* set initial values. */
    tmp->alloc = alloc;
    tmp->type = VCJSON_VALUE_TYPE_STRING;
    tmp->value = string;

    /* success. */
    *value = tmp;
    retval = STATUS_SUCCESS;
    goto done;

done:
    return retval;
}
