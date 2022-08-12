/**
 * \file vcjson_object_element_resource_release.c
 *
 * \brief Release an object element's resource.
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
 * \brief Release a \ref vcjson_object_element.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_element_resource_release(RCPR_SYM(resource)* r)
{
    status key_reclaim_retval = STATUS_SUCCESS;
    status value_release_retval = STATUS_SUCCESS;
    status reclaim_retval = STATUS_SUCCESS;
    vcjson_object_element* elem = (vcjson_object_element*)r;

    /* cache allocator. */
    allocator* alloc = elem->alloc;

    /* if the key is set, reclaim it. */
    if (NULL != elem->key)
    {
        /* clear key. */
        size_t key_len = strlen(elem->key);
        memset(elem->key, 0, key_len);

        /* reclaim key memory. */
        key_reclaim_retval = allocator_reclaim(alloc, elem->key);
    }

    /* if the value is set, release it. */
    if (NULL != elem->value)
    {
        value_release_retval = resource_release(&elem->value->hdr);
    }

    /* clear elem. */
    memset(elem, 0, sizeof(*elem));

    /* reclaim elem. */
    reclaim_retval = allocator_reclaim(alloc, elem);

    /* decode return value. */
    if (STATUS_SUCCESS != key_reclaim_retval)
    {
        return key_reclaim_retval;
    }
    else if (STATUS_SUCCESS != value_release_retval)
    {
        return value_release_retval;
    }
    else
    {
        return reclaim_retval;
    }
}
