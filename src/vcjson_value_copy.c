/**
 * \file vcjson_value_copy.c
 *
 * \brief Copy a value instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Create a deep copy of the given \ref vcjson_value instance.
 *
 * \note On success, this function creates a \ref vcjson_value instance. This
 * value instance is a resource that is owned by the caller. When no longer
 * needed, this resource must be released by calling \ref resource_release on
 * its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original value to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_copy(
    vcjson_value** value, RCPR_SYM(allocator)* alloc, const vcjson_value* orig)
{
    /* TODO - stub this out with an always failing method so we can write object
     * and array copy first. */
    (void)value;
    (void)alloc;
    (void)orig;
    return -1;
}
