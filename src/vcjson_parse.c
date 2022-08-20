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
static status
    vcjson_string_simplify(
        char* output, size_t output_len, size_t* simplified_len,
        const char* input, size_t input_length);

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

    /* compute the maximum string size. */
    buffer_size = (*error_end + 1) - *error_begin - 2;
    printf("%lu:%lu:%lu\n", *error_end, *error_begin, buffer_size);
    fflush(stdout);

    /* create a working buffer. */
    retval = allocator_allocate(alloc, (void**)&buffer, buffer_size + 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* simplify the string value. */
    size_t length;
    retval =
        vcjson_string_simplify(
            buffer, buffer_size + 1, &length, input + *error_begin + 1,
            buffer_size);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_buffer;
    }

    /* create a string from this value. */
    retval =
        vcjson_string_create_from_raw(&string_value, alloc, buffer, length);
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
    if (STATUS_SUCCESS != release_retval)
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

/**
 * \brief Convert a JSON string value from input to a raw C string value.
 *
 * This method converts escape codes into their raw values and converts UTF-16
 * values and surrogate pairs provided as u escape codes and pairs to UTF-8
 * values.
 *
 * \param output            Buffer to hold the output string.
 * \param output_len        The maximum output length.
 * \param simplified_len    The length of the simplified string.
 * \param input             The input string.
 * \param input_length      The length of the input string.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_string_simplify(
        char* output, size_t output_len, size_t* simplified_len,
        const char* input, size_t input_length)
{
    *simplified_len = 0;

    for (size_t i = 0; i < input_length; ++i)
    {
        char ch = input[i];

        /* only copy at max output_len characters. */
        if (*simplified_len >= output_len)
        {
            return STATUS_SUCCESS;
        }

        switch (ch)
        {
            /* is this the beginning of an escape sequence? */
            case '\\':
                if (i+1 < input_length)
                {
                    char escape = input[i+1];
                    switch (escape)
                    {
                        case 'b':
                            output[*simplified_len] = '\b';
                            ++(*simplified_len);
                            ++i;
                            break;

                        case 'f':
                            output[*simplified_len] = '\f';
                            ++(*simplified_len);
                            ++i;
                            break;

                        case 'n':
                            output[*simplified_len] = '\n';
                            ++(*simplified_len);
                            ++i;
                            break;

                        case 'r':
                            output[*simplified_len] = '\r';
                            ++(*simplified_len);
                            ++i;
                            break;

                        case 't':
                            output[*simplified_len] = '\t';
                            ++(*simplified_len);
                            ++i;
                            break;

                        case '\\':
                            output[*simplified_len] = '\\';
                            ++(*simplified_len);
                            ++i;
                            break;

                        case '/':
                            output[*simplified_len] = '/';
                            ++(*simplified_len);
                            ++i;
                            break;

                        case '"':
                            output[*simplified_len] = '"';
                            ++(*simplified_len);
                            ++i;
                            break;

                        default:
                        return
                        ERROR_VCJSON_PARSE_40331c16_1a5d_4b56_984b_e9f3b65c5661;
                    }
                }
                else
                {
                    /* cut-off escape sequence. */
                    return
                        ERROR_VCJSON_PARSE_4a0c973b_8689_4b34_895e_f494e2c325fb;
                }
                break;

            default:
                output[*simplified_len] = ch;
                ++(*simplified_len);
                break;
        }
    }

    /* add the ASCII zero. */
    if (*simplified_len < output_len)
    {
        output[*simplified_len] = 0;
        ++(*simplified_len);
    }

    /* success. */
    return STATUS_SUCCESS;
}
