/**
 * \file vcjson_object_iterator_resource_release.c
 *
 * \brief Release an object iterator resource.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;

/**
 * \brief Release a \ref vcjson_object_iterator.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_resource_release(RCPR_SYM(resource)* r)
{
    vcjson_object_iterator* iter = (vcjson_object_iterator*)r;

    /* cache allocator. */
    allocator* alloc = iter->alloc;

    /* clear instance memory. */
    memset(iter, 0, sizeof(*iter));

    /* reclaim memory. */
    return
        allocator_reclaim(alloc, iter);
}
