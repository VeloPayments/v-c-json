/**
 * \file vcjson_null_resource_release.c
 *
 * \brief Release a \ref vcjson_null resource.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Release a \ref vcjson_null resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_null_resource_release(RCPR_SYM(resource)* /*r*/)
{
    /* NULL is a singleton. Releasing its resource has no meaning. */
    return STATUS_SUCCESS;
}
