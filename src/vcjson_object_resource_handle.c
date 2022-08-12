/**
 * \file vcjson_object_resource_handle.c
 *
 * \brief Get the resource handle of a \ref vcjson_object instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the resource handle for the given \ref vcjson_object instance.
 *
 * \param obj           The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_object_resource_handle(vcjson_object* obj)
{
    return &obj->hdr;
}
