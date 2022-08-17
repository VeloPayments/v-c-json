/**
 * \file vcjson_parse_string.c
 *
 * \brief Create a JSON value from a string.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Attempt to parse a JSON value from a UTF-8 string.
 *
 * \note This parse function must consume all input to be successful.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param error_begin   Pointer to receive the start of an error location on
 *                      failure.
 * \param error_end     Pointer to receive the end of an error location on
 *                      failure.
 * \param alloc         The allocator to use for this operation.
 * \param input         The UTF-8 input string.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_parse_string(
    vcjson_value** value, size_t* error_begin, size_t* error_end,
    RCPR_SYM(allocator)* alloc, const char* input)
{
    return
        vcjson_parse(
            value, error_begin, error_end, alloc, input, strlen(input));
}
