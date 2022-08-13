/**
 * \file vcjson_array_resource_release.c
 *
 * \brief Release an array resource.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/**
 * \brief Release a \ref vcjson_array.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_array_resource_release(RCPR_SYM(resource)* r)
{
    status error_retval = STATUS_SUCCESS;
    status retval = STATUS_SUCCESS;
    vcjson_array* arr = (vcjson_array*)r;

    /* cache allocator. */
    allocator* alloc = arr->alloc;

    /* if the array is set, iterate through elements. */
    if (NULL != arr->arr)
    {
        for (size_t i = 0; i < arr->elems; ++i)
        {
            retval = resource_release(&arr->arr[i]->hdr);
            if (STATUS_SUCCESS != retval)
            {
                error_retval = retval;
            }
        }
    }

    /* clear structure. */
    memset(arr, 0, sizeof(*arr));

    /* reclaim structure. */
    retval = allocator_reclaim(alloc, arr);
    if (STATUS_SUCCESS != retval)
    {
        error_retval = retval;
    }

    /* return an error if any errors were found. */
    return error_retval;
}
