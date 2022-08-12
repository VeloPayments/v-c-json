/**
 * \file vcjson_object_resource_release.c
 *
 * \brief Release an object resource.
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
 * \brief Release a \ref vcjson_object.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_resource_release(RCPR_SYM(resource)* r)
{
    status reclaim_retval = STATUS_SUCCESS;
    status elements_release_retval = STATUS_SUCCESS;
    vcjson_object* obj = (vcjson_object*)r;

    /* cache allocator. */
    allocator* alloc = obj->alloc;

    /* release elements resource if set. */
    if (NULL != obj->elements)
    {
        elements_release_retval =
            resource_release(rbtree_resource_handle(obj->elements));
    }

    /* clear structure. */
    memset(obj, 0, sizeof(*obj));

    /* reclaim memory. */
    reclaim_retval = allocator_reclaim(alloc, obj);

    /* decode return value. */
    if (STATUS_SUCCESS != elements_release_retval)
    {
        return elements_release_retval;
    }
    else
    {
        return reclaim_retval;
    }
}
