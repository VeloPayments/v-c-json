/**
 * \file vcjson_number_copy.c
 *
 * \brief Copy a number instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Make a deep copy of the given \ref vcjson_number instance.
 *
 * \note On success, this function creates a \ref vcjson_number instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param number        Pointer to the number pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original number to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_number_copy(
    vcjson_number** number, RCPR_SYM(allocator)* alloc,
    const vcjson_number* orig)
{
    return
        vcjson_number_create(number, alloc, orig->value);
}
