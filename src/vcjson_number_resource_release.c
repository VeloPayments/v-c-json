/**
 * \file vcjson_number_resource_release.c
 *
 * \brief Release a number resource.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;

/**
 * \brief Release a \ref vcjson_number resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_number_resource_release(RCPR_SYM(resource)* r)
{
    vcjson_number* number = (vcjson_number*)r;

    /* cache allocator. */
    allocator* alloc = number->alloc;

    /* clear structure. */
    memset(number, 0, sizeof(*number));

    /* reclaim memory. */
    return
        allocator_reclaim(alloc, number);
}
