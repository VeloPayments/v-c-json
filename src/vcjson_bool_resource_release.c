/**
 * \file vcjson_bool_resource_release.c
 *
 * \brief Release a \ref vcjson_bool resource.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Release a \ref vcjson_bool resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_bool_resource_release(RCPR_SYM(resource)* /*r*/)
{
    /* TRUE and FALSE are singletons. Releasing them has no meaning. */
    return STATUS_SUCCESS;
}
