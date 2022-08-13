/**
 * \file vcjson_string_copy.c
 *
 * \brief Copy a string instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Make a deep copy of the given \ref vcjson_string instance.
 *
 * \note On success, this function creates a \ref vcjson_string instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param string        Pointer to the string pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original string instance to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_string_copy(
    vcjson_string** string, RCPR_SYM(allocator)* alloc,
    const vcjson_string* orig)
{
    return
        vcjson_string_create(string, alloc, orig->value);
}
