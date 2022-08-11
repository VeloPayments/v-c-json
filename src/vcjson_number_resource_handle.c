/**
 * \file vcjson_number_resource_handle.c
 *
 * \brief Return a resource handle for a number resource.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the resource handle for the given \ref vcjson_number instance.
 *
 * \param number        The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_number_resource_handle(vcjson_number* number)
{
    return &number->hdr;
}
