/**
 * \file vcjson_internal.h
 *
 * \brief Internal header for vcjson.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

#include <rcpr/rbtree.h>
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

struct vcjson_object
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(rbtree)* elements;
};

/**
 * \brief An element in a JSON object.
 */
typedef struct vcjson_object_element vcjson_object_element;

struct vcjson_object_element
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    char* key;
    vcjson_value* value;
};

struct vcjson_object_iterator
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(rbtree)* tree;
    RCPR_SYM(rbtree_node)* nil;
    RCPR_SYM(rbtree_node)* curr;
};

struct vcjson_array
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    vcjson_value** arr;
    size_t elems;
};

extern vcjson_null VCJSON_NULL_IMPL;
extern vcjson_bool VCJSON_BOOL_TRUE_IMPL;
extern vcjson_bool VCJSON_BOOL_FALSE_IMPL;

/**
 * \brief Comparison function for an object elements tree.
 *
 * \param context       Unused.
 * \param lhs           The left-hand-side key to compare.
 * \param rhs           The right-hand-side key to compare.
 *
 * \returns an integer value representing the comparison result.
 *      - RCPR_COMPARE_LT if \p lhs &lt; \p rhs.
 *      - RCPR_COMPARE_EQ if \p lhs == \p rhs.
 *      - RCPR_COMPARE_GT if \p lhs &gt; \p rhs.
 */
RCPR_SYM(rcpr_comparison_result) vcjson_object_element_compare(
    void* context, const void* lhs, const void* rhs);

/**
 * \brief Given an object element resource, return the key for this resource.
 *
 * \param context       Unused.
 * \param r             The resource.
 *
 * \returns the key for this resource.
 */
const void* vcjson_object_element_key(
    void* context, const RCPR_SYM(resource)* r);

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

/**
 * \brief Release a \ref vcjson_value with a resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_value_with_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_object.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_object_element.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_element_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_object_iterator.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_array.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_array_resource_release(RCPR_SYM(resource)* r);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
