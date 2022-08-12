/**
 * \file vcjson_value_with_resource_release.c
 *
 * \brief Release a value with resource.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/**
 * \brief Release a \ref vcjson_value with a resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_value_with_resource_release(RCPR_SYM(resource)* r)
{
    status reclaim_retval = STATUS_SUCCESS;
    status value_release_retval = STATUS_SUCCESS;
    vcjson_value* value = (vcjson_value*)r;

    /* cache allocator. */
    allocator* alloc = value->alloc;

    /* release the value associated with this value. */
    if (NULL != value->value)
    {
        resource* value_resource = (resource*)value->value;
        value_release_retval = resource_release(value_resource);
    }

    /* clear structure. */
    memset(value, 0, sizeof(*value));

    /* reclaim memory. */
    reclaim_retval = allocator_reclaim(alloc, value);

    /* decode return value. */
    if (STATUS_SUCCESS != value_release_retval)
    {
        return value_release_retval;
    }
    else
    {
        return reclaim_retval;
    }
}
