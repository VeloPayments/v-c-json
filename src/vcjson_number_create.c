/**
 * \file vcjson_number_create.c
 *
 * \brief Create a number instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/**
 * \brief Create a \ref vcjson_number instance using the given allocator and
 * numeric value.
 *
 * \note On success, this function creates a \ref vcjson_number instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param number        Pointer to the number pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param value         The number value to be used for this instance.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_number_create(
    vcjson_number** number, RCPR_SYM(allocator)* alloc, double value)
{
    status retval;
    vcjson_number* tmp;

    /* allocate memory for this number instance. */
    retval = allocator_allocate(alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear out this structure. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize resource. */
    resource_init(&tmp->hdr, &vcjson_number_resource_release);

    /* set values. */
    tmp->alloc = alloc;
    tmp->value = value;

    /* success. */
    *number = tmp;
    retval = STATUS_SUCCESS;
    goto done;
    
done:
    return retval;
}
