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

struct vcjson_bool
{
    RCPR_SYM(resource) hdr;
    bool value;
};

struct vcjson_value
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    int type;
    void* value;
};

extern vcjson_null VCJSON_NULL_IMPL;
extern vcjson_bool VCJSON_BOOL_TRUE_IMPL;
extern vcjson_bool VCJSON_BOOL_FALSE_IMPL;

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

/**
 * \brief Release a \ref vcjson_bool resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_bool_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_value singleton resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_value_singleton_resource_release(RCPR_SYM(resource)* r);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
