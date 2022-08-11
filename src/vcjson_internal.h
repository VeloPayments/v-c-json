/**
 * \file vcjson_internal.h
 *
 * \brief Internal header for vcjson.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

#include <rcpr/resource/protected.h>
#include <rcpr/status.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

struct vcjson_number
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    double value;
};

struct vcjson_string
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    char* value;
    size_t length;
};

struct vcjson_null
{
    RCPR_SYM(resource) hdr;
};

/**
 * \brief Release a \ref vcjson_number resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_number_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_string resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_string_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_null resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_null_resource_release(RCPR_SYM(resource)* r);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
