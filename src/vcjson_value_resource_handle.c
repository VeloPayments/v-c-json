/**
 * \file vcjson_value_resource_handle.c
 *
 * \brief Get the resource handle of the \ref vcjson_value instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the resource handle for the given \ref vcjson_value instance.
 *
 * \param value         The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_value_resource_handle(vcjson_value* value)
{
    return &value->hdr;
}
