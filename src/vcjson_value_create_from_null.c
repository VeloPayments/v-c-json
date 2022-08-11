/**
 * \file vcjson_value_create_from_null.c
 *
 * \brief Create the null \ref vcjson_value singleton instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

static vcjson_value VCJSON_VALUE_NULL_SINGLETON_IMPL = {
    .hdr = { .release = &vcjson_value_singleton_resource_release },
    .alloc = NULL,
    .type = VCJSON_VALUE_TYPE_NULL,
    .value = &VCJSON_NULL_IMPL
};

static vcjson_value* VCJSON_VALUE_NULL_SINGLETON =
    &VCJSON_VALUE_NULL_SINGLETON_IMPL;

/**
 * \brief Create a \ref vcjson_value instance from the null singleton.
 *
 * \note On success, this function creates a \ref vcjson_value instance which
 * references the null singleton. This value instance is a resource that is
 * owned by the caller. When no longer needed, this resource must be released by
 * calling \ref resource_release on its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_null(
    vcjson_value** value, RCPR_SYM(allocator)* /*alloc*/)
{
    /* assign the null singleton value to the value. */
    *value = VCJSON_VALUE_NULL_SINGLETON;

    return STATUS_SUCCESS;
}
