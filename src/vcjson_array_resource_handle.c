/**
 * \file vcjson_array_resource_handle.c
 *
 * \brief Get the resource handle for this array instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the resource handle for the given \ref vcjson_array instance.
 *
 * \param arr           The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_array_resource_handle(vcjson_array* arr)
{
    return &arr->hdr;
}
