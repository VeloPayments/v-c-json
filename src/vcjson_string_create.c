/**
 * \file vcjson_string_create.c
 *
 * \brief Create a string instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/**
 * \brief Create a \ref vcjson_string instance using the given allocator and
 * string value.
 *
 * \note On success, this function creates a \ref vcjson_string instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param string        Pointer to the string pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param value         The string value to be used for this instance. This
 *                      value is copied; the original value is assumed owned by
 *                      the caller.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_string_create(
    vcjson_string** string, RCPR_SYM(allocator)* alloc, const char* value)
{
    /* get the length of the string. */
    size_t length = strlen(value) + 1;

    return vcjson_string_create_from_raw(string, alloc, value, length);
}
