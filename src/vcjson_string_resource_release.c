/**
 * \file vcjson_string_resource_release.c
 *
 * \brief Release a \ref vcjson_string resource.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;

/**
 * \brief Release a \ref vcjson_string resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_string_resource_release(RCPR_SYM(resource)* r)
{
    status string_reclaim_retval = STATUS_SUCCESS;
    status reclaim_retval = STATUS_SUCCESS;
    vcjson_string* string = (vcjson_string*)r;

    /* cache allocator. */
    allocator* alloc = string->alloc;

    /* clear string value if set. */
    if (NULL != string->value)
    {
        memset(string->value, 0, string->length);
        string_reclaim_retval = allocator_reclaim(alloc, string->value);
    }

    /* clear string structure. */
    memset(string, 0, sizeof(*string));

    /* reclaim string structure. */
    reclaim_retval = allocator_reclaim(alloc, string);

    /* decode status code. */
    if (STATUS_SUCCESS != string_reclaim_retval)
    {
        return string_reclaim_retval;
    }
    else
    {
        return reclaim_retval;
    }
}
