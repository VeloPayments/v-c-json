/**
 * \file vcjson_string_resource_handle.c
 *
 * \brief Get the resource handle of a \ref vcjson_string instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the resource handle for the given \ref vcjson_string instance.
 *
 * \param string        The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_string_resource_handle(vcjson_string* string)
{
    return &string->hdr;
}
