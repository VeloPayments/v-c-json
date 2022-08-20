/**
 * \file vcjson_parse.c
 *
 * \brief Create a JSON value from a character buffer.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/* forward decls. */
static status
    vcjson_read_value(
        vcjson_value** value, size_t* error_begin, size_t* error_end,
        allocator* alloc, const char* input, size_t size, size_t* offset);
static status
    vcjson_read_value_true(
        vcjson_value** value, size_t* error_begin, size_t* error_end,
        allocator* alloc, const char* input, size_t size, size_t* offset);
static status
    vcjson_read_value_false(
        vcjson_value** value, size_t* error_begin, size_t* error_end,
        allocator* alloc, const char* input, size_t size, size_t* offset);
static status
    vcjson_read_value_null(
        vcjson_value** value, size_t* error_begin, size_t* error_end,
        allocator* alloc, const char* input, size_t size, size_t* offset);
static status
    vcjson_read_value_string(
        vcjson_value** value, size_t* error_begin, size_t* error_end,
        allocator* alloc, const char* input, size_t size, size_t* offset);

/**
 * \brief Attempt to parse a JSON value from a UTF-8 character buffer.
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
 * \param input         The input UTF-8 character buffer.
 * \param size          The size of this buffer.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_parse(
    vcjson_value** value, size_t* error_begin, size_t* error_end,
    RCPR_SYM(allocator)* alloc, const char* input, size_t size)
{
    status retval, release_retval;
    int symbol;
    size_t offset;

    /* initialize the positions as 0 to start the parse. */
    *error_begin = *error_end = offset = 0;

    /* read a value. */
    retval =
        vcjson_read_value(
            value, error_begin, error_end, alloc, input, size, &offset);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* scan the next token. */
    retval =
        vcjson_scan_symbol(
            &symbol, error_begin, error_end, input, size, &offset);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_value;
    }

    /* verify that this token is EOF. */
    if (VCJSON_LEXER_SYMBOL_SPECIAL_EOF != symbol)
    {
        retval = ERROR_VCJSON_PARSE_b87aa047_46c8_453c_aa3c_bb7c1dd70402;
        goto cleanup_value;
    }

    /* success. */
    retval = STATUS_SUCCESS;
    goto done;

cleanup_value:
    release_retval = resource_release(vcjson_value_resource_handle(*value));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Create a true value.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param error_begin   Pointer to receive the start of an error location on
 *                      failure.
 * \param error_end     Pointer to receive the end of an error location on
 *                      failure.
 * \param alloc         The allocator to use for this operation.
 * \param input         The input UTF-8 character buffer.
 * \param size          The size of this buffer.
 * \param offset        The current offset in the input buffer.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_true(
        vcjson_value** value, size_t* /*error_begin*/, size_t* /*error_end*/,
        allocator* alloc, const char* /*input*/, size_t /*size*/,
        size_t* /*offset*/)
{
    status retval;

    /* create the true instance. */
    retval = vcjson_value_create_from_true(value, alloc);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* success. */
    return STATUS_SUCCESS;
}

/**
 * \brief Create a false value.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param error_begin   Pointer to receive the start of an error location on
 *                      failure.
 * \param error_end     Pointer to receive the end of an error location on
 *                      failure.
 * \param alloc         The allocator to use for this operation.
 * \param input         The input UTF-8 character buffer.
 * \param size          The size of this buffer.
 * \param offset        The current offset in the input buffer.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_false(
        vcjson_value** value, size_t* /*error_begin*/, size_t* /*error_end*/,
        allocator* alloc, const char* /*input*/, size_t /*size*/,
        size_t* /*offset*/)
{
    status retval;

    /* create the false instance. */
    retval = vcjson_value_create_from_false(value, alloc);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* success. */
    return STATUS_SUCCESS;
}

/**
 * \brief Create a null value.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param error_begin   Pointer to receive the start of an error location on
 *                      failure.
 * \param error_end     Pointer to receive the end of an error location on
 *                      failure.
 * \param alloc         The allocator to use for this operation.
 * \param input         The input UTF-8 character buffer.
 * \param size          The size of this buffer.
 * \param offset        The current offset in the input buffer.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_null(
        vcjson_value** value, size_t* /*error_begin*/, size_t* /*error_end*/,
        allocator* alloc, const char* /*input*/, size_t /*size*/,
        size_t* /*offset*/)
{
    status retval;

    /* create the null instance. */
    retval = vcjson_value_create_from_null(value, alloc);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* success. */
    return STATUS_SUCCESS;
}

/**
 * \brief Create a string value from input.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param error_begin   Pointer to receive the start of an error location on
 *                      failure.
 * \param error_end     Pointer to receive the end of an error location on
 *                      failure.
 * \param alloc         The allocator to use for this operation.
 * \param input         The input UTF-8 character buffer.
 * \param size          The size of this buffer.
 * \param offset        The current offset in the input buffer.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_string(
        vcjson_value** value, size_t* error_begin, size_t* error_end,
        allocator* alloc, const char* input, size_t /*size*/,
        size_t* /*offset*/)
{
    status retval, release_retval;
    vcjson_string* string_value;
    char* buffer;
    size_t buffer_size;

    /* TODO - JSON strings need to be changed to deal with raw buffer values. */

    /* compute the maximum string size. */
    buffer_size = (*error_end + 1) - *error_begin - 2;
    printf("%lu:%lu:%lu\n", *error_end, *error_begin, buffer_size);
    fflush(stdout);

    /* duplicate the string value to create a working buffer. */
    retval = allocator_allocate(alloc, (void**)&buffer, buffer_size + 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* copy the string value. */
    memcpy(buffer, input + *error_begin + 1, buffer_size);
    buffer[buffer_size] = 0;

    /* create a string from this value. */
    retval = vcjson_string_create(&string_value, alloc, buffer);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_buffer;
    }

    /* create a value from this string value. */
    retval = vcjson_value_create_from_string(value, alloc, string_value);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_string_value;
    }

    /* success. */
    retval = STATUS_SUCCESS;
    goto cleanup_buffer;

cleanup_string_value:
    release_retval =
        resource_release(vcjson_string_resource_handle(string_value));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

cleanup_buffer:
    memset(buffer, 0, buffer_size);
    release_retval = allocator_reclaim(alloc, buffer);
    if (STATUS_SUCCESS != retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Read a single JSON value from input.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param error_begin   Pointer to receive the start of an error location on
 *                      failure.
 * \param error_end     Pointer to receive the end of an error location on
 *                      failure.
 * \param alloc         The allocator to use for this operation.
 * \param input         The input UTF-8 character buffer.
 * \param size          The size of this buffer.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value(
        vcjson_value** value, size_t* error_begin, size_t* error_end,
        allocator* alloc, const char* input, size_t size, size_t* offset)
{
    status retval;
    int symbol;

    /* scan for a symbol. */
    retval =
        vcjson_scan_symbol(
            &symbol, error_begin, error_end, input, size, offset);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* decode the symbol received. */
    switch (symbol)
    {
        /* we need a valid JSON value for this read to be successful. */
        case VCJSON_LEXER_SYMBOL_SPECIAL_EOF:
            return ERROR_VCJSON_PARSE_b369f991_4e11_4210_9076_ddc799d5bf44;

        /* read a true literal. */
        case VCJSON_LEXER_SYMBOL_TRUE:
            return
                vcjson_read_value_true(
                    value, error_begin, error_end, alloc, input, size, offset);

        /* read a false literal. */
        case VCJSON_LEXER_SYMBOL_FALSE:
            return
                vcjson_read_value_false(
                    value, error_begin, error_end, alloc, input, size, offset);

        /* read a null literal. */
        case VCJSON_LEXER_SYMBOL_NULL:
            return
                vcjson_read_value_null(
                    value, error_begin, error_end, alloc, input, size, offset);

        /* read a string literal. */
        case VCJSON_LEXER_SYMBOL_STRING:
            return
                vcjson_read_value_string(
                    value, error_begin, error_end, alloc, input, size, offset);

        /* an unknown symbol was encountered. */
        default:
            return ERROR_VCJSON_PARSE_fb48555e_2ed9_414a_841e_0d5b39b52090;
    }
}
