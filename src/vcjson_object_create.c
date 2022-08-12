/**
 * \file vcjson_object_create.c
 *
 * \brief Create an object instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_rbtree;
RCPR_IMPORT_resource;

/**
 * \brief Create an empty \ref vcjson_object using the given allocator.
 *
 * \note On success, this function creates a  \ref vcjson_object instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param obj           Pointer to the object pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_create(vcjson_object** obj, RCPR_SYM(allocator)* alloc)
{
    status retval, release_retval;
    vcjson_object* tmp;

    /* allocate memory for the object instance. */
    retval = allocator_allocate(alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear instance. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize resource. */
    resource_init(&tmp->hdr, &vcjson_object_resource_release);

    /* set initial values. */
    tmp->alloc = alloc;

    /* create rbtree instance for this object. */
    retval =
        rbtree_create(
            &tmp->elements, alloc, &vcjson_object_element_compare,
            &vcjson_object_element_key, NULL);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. */
    *obj = tmp;
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
