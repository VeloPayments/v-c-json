/**
 * \file vcjson_parse.c
 *
 * \brief Create a JSON value from a character buffer.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <rcpr/slist.h>
#include <stdlib.h>
#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;
RCPR_IMPORT_slist;

/* forward decls. */
static status
    vcjson_read_value(
        vcjson_value** value, vcjson_parser_context* ctx);
static status
    vcjson_read_value_true(
        vcjson_value** value, vcjson_parser_context* ctx);
static status
    vcjson_read_value_false(
        vcjson_value** value, vcjson_parser_context* ctx);
static status
    vcjson_read_value_null(
        vcjson_value** value, vcjson_parser_context* ctx);
static status
    vcjson_read_value_string(
        vcjson_value** value, vcjson_parser_context* ctx);
static status
    vcjson_read_string(
        vcjson_string** string, vcjson_parser_context* ctx);
static status
    vcjson_read_value_object(
        vcjson_value** value, vcjson_parser_context* ctx);
static status
    vcjson_read_value_object_member(
        vcjson_object* obj, vcjson_parser_context* ctx);
static status
    vcjson_read_value_array(
        vcjson_value** value, vcjson_parser_context* ctx);
static status
    vcjson_read_array_from_list(
        vcjson_value** value, allocator* alloc, slist* list);
static status
    vcjson_read_value_number(
        vcjson_value** value, vcjson_parser_context* ctx);
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
    vcjson_parser_context ctx;

    /* initialize the positions as 0 to start the parse. */
    *error_begin = *error_end = offset = 0;

    /* initialize the parser context. */
    ctx.alloc = alloc;
    ctx.error_begin = error_begin;
    ctx.error_end = error_end;
    ctx.input = input;
    ctx.size = size;
    ctx.offset = &offset;
    ctx.recursion_depth = 0;

    /* read a value. */
    retval = vcjson_read_value(value, &ctx);
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
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_true(
        vcjson_value** value, vcjson_parser_context* ctx)
{
    status retval;

    /* create the true instance. */
    retval = vcjson_value_create_from_true(value, ctx->alloc);
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
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_false(
        vcjson_value** value, vcjson_parser_context* ctx)
{
    status retval;

    /* create the false instance. */
    retval = vcjson_value_create_from_false(value, ctx->alloc);
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
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_null(
        vcjson_value** value, vcjson_parser_context* ctx)
{
    status retval;

    /* create the null instance. */
    retval = vcjson_value_create_from_null(value, ctx->alloc);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* success. */
    return STATUS_SUCCESS;
}

/**
 * \brief Create a number value from input.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_number(
        vcjson_value** value, vcjson_parser_context* ctx)
{
    status retval, release_retval;
    vcjson_number* number;
    char* buffer;
    size_t buffer_size;

    /* compute the working string size. */
    buffer_size = (*ctx->error_end + 1) - *ctx->error_begin;

    /* create a working buffer. */
    retval = allocator_allocate(ctx->alloc, (void**)&buffer, buffer_size + 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* copy the string value. */
    memcpy(buffer, ctx->input + *ctx->error_begin, buffer_size);
    buffer[buffer_size] = 0;

    /* convert this to a number. */
    double numberval = atof(buffer);

    /* create a JSON number. */
    retval = vcjson_number_create(&number, ctx->alloc, numberval);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_buffer;
    }

    /* create a JSON value. */
    retval = vcjson_value_create_from_number(value, ctx->alloc, number);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_number;
    }

    /* success. */
    retval = STATUS_SUCCESS;
    goto cleanup_buffer;

cleanup_number:
    release_retval = resource_release(vcjson_number_resource_handle(number));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

cleanup_buffer:
    memset(buffer, 0, buffer_size);
    release_retval = allocator_reclaim(ctx->alloc, buffer);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Create a string value from input.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_string(
        vcjson_value** value, vcjson_parser_context* ctx)
{
    status retval, release_retval;
    vcjson_string* string_value;

    /* read the string. */
    retval = vcjson_read_string(&string_value, ctx);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* create a value from this string value. */
    retval = vcjson_value_create_from_string(value, ctx->alloc, string_value);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_string_value;
    }

    /* success. */
    retval = STATUS_SUCCESS;
    goto done;

cleanup_string_value:
    release_retval =
        resource_release(vcjson_string_resource_handle(string_value));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Create a string from input.
 *
 * \param string        Pointer to the string pointer to hold the JSON string on
 *                      success.
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_string(
        vcjson_string** string, vcjson_parser_context* ctx)
{
    status retval, release_retval;
    char* buffer;
    size_t buffer_size;

    /* compute the maximum string size. */
    buffer_size = (*ctx->error_end + 1) - *ctx->error_begin - 2;

    /* create a working buffer. */
    retval = allocator_allocate(ctx->alloc, (void**)&buffer, buffer_size + 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* simplify the string value. */
    size_t length;
    retval =
        vcjson_string_simplify(
            buffer, buffer_size + 1, &length,
            ctx->input + *ctx->error_begin + 1, buffer_size);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_buffer;
    }

    /* create a string from this value. */
    retval =
        vcjson_string_create_from_raw(string, ctx->alloc, buffer, length);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_buffer;
    }

    /* success. */
    retval = STATUS_SUCCESS;
    goto cleanup_buffer;

cleanup_buffer:
    memset(buffer, 0, buffer_size);
    release_retval = allocator_reclaim(ctx->alloc, buffer);
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
 * \param ctx           The context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value(
        vcjson_value** value, vcjson_parser_context* ctx)
{
    status retval;
    int symbol;

    /* check the recursion depth. */
    if (ctx->recursion_depth >= VCJSON_PARSER_MAXIMUM_RECURSION_DEPTH)
    {
        return ERROR_VCJSON_PARSE_RECURSION_DEPTH_EXCEEDED;
    }
    else
    {
        ++ctx->recursion_depth;
    }

    /* scan for a symbol. */
    retval =
        vcjson_scan_symbol(
            &symbol, ctx->error_begin, ctx->error_end, ctx->input, ctx->size,
            ctx->offset);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* decode the symbol received. */
    switch (symbol)
    {
        /* we need a valid JSON value for this read to be successful. */
        case VCJSON_LEXER_SYMBOL_SPECIAL_EOF:
            retval = ERROR_VCJSON_PARSE_b369f991_4e11_4210_9076_ddc799d5bf44;
            goto done;

        /* read a true literal. */
        case VCJSON_LEXER_SYMBOL_TRUE:
            retval = vcjson_read_value_true(value, ctx);
            goto done;

        /* read a false literal. */
        case VCJSON_LEXER_SYMBOL_FALSE:
            retval = vcjson_read_value_false(value, ctx);
            goto done;

        /* read a null literal. */
        case VCJSON_LEXER_SYMBOL_NULL:
            retval = vcjson_read_value_null(value, ctx);
            goto done;

        /* read a number literal. */
        case VCJSON_LEXER_SYMBOL_NUMBER:
            retval = vcjson_read_value_number(value, ctx);
            goto done;

        /* read a string literal. */
        case VCJSON_LEXER_SYMBOL_STRING:
            retval = vcjson_read_value_string(value, ctx);
            goto done;

        /* read an object. */
        case VCJSON_LEXER_PRIM_LEFT_BRACE:
            retval = vcjson_read_value_object(value, ctx);
            goto done;

        /* read an array. */
        case VCJSON_LEXER_PRIM_LEFT_BRACKET:
            retval = vcjson_read_value_array(value, ctx);
            goto done;

        /* an unknown symbol was encountered. */
        default:
            retval = ERROR_VCJSON_PARSE_fb48555e_2ed9_414a_841e_0d5b39b52090;
            goto done;
    }

done:
    --ctx->recursion_depth;

    return retval;
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
    }

    /* success. */
    return STATUS_SUCCESS;
}

/**
 * \brief Create an object value from input.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_object(
        vcjson_value** value, vcjson_parser_context* ctx)
{
    status retval, release_retval;
    int symbol;
    vcjson_object* obj;
    bool expecting_comma = false;

    /* create an empty object instance. */
    retval = vcjson_object_create(&obj, ctx->alloc);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* iterate through the object members. */
    for (;;)
    {
        /* scan the next symbol. */
        retval =
            vcjson_scan_symbol(
                &symbol, ctx->error_begin, ctx->error_end, ctx->input,
                ctx->size, ctx->offset);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_obj;
        }

        /* decode the symbol read. */
        switch (symbol)
        {
            /* this is the end of the object. */
            case VCJSON_LEXER_PRIM_RIGHT_BRACE:
                /* if the object is empty or the last member did not have a
                 * comma. */
                if (vcjson_object_elements(obj) == 0 || expecting_comma)
                {
                    retval =
                        vcjson_value_create_from_object(value, ctx->alloc, obj);
                    if (STATUS_SUCCESS != retval)
                    {
                        goto cleanup_obj;
                    }

                    /* success. */
                    retval = STATUS_SUCCESS;
                    goto done;
                }
                else
                {
                    /* we have a dangling comma. */
                    retval =
                        ERROR_VCJSON_PARSE_69c86e4f_d981_402d_a4fd_c051b97e821a;
                    goto cleanup_obj;
                }

            /* this is the start of a member. */
            case VCJSON_LEXER_SYMBOL_STRING:
                if (expecting_comma)
                {
                    retval =
                        ERROR_VCJSON_PARSE_1e9e755f_b416_4f9a_95e7_5acd39a09b47;
                    goto cleanup_obj;
                }
                else
                {
                    /* read the object member. */
                    retval = vcjson_read_value_object_member(obj, ctx);
                    if (STATUS_SUCCESS != retval)
                    {
                        goto cleanup_obj;
                    }

                    /* we need a comma next. */
                    expecting_comma = true;
                }
                break;

            /* parse a comma between members. */
            case VCJSON_LEXER_PRIM_COMMA:
                if (expecting_comma)
                {
                    expecting_comma = false;
                }
                else
                {
                    retval =
                        ERROR_VCJSON_PARSE_b664370d_72ce_4778_8f68_30c7dc3b14e5;
                    goto cleanup_obj;
                }
                break;

            default:
                retval =
                    ERROR_VCJSON_PARSE_ffa4f503_8429_49f4_bbf2_8a91276d234c;
                goto cleanup_obj;
        }
    }

cleanup_obj:
    release_retval = resource_release(vcjson_object_resource_handle(obj));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Read an object member key-value pair, starting with the string at the
 * current begin:end pair.
 *
 * \param obj           Pointer to the object to update with this key-value
 *                      pair.
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_read_value_object_member(
    vcjson_object* obj, vcjson_parser_context* ctx)
{
    status retval, release_retval;
    int symbol;
    vcjson_string* elemkey;
    vcjson_value* elemvalue;

    /* read the key string. */
    retval = vcjson_read_string(&elemkey, ctx);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* scan the next symbol. */
    retval =
        vcjson_scan_symbol(
            &symbol, ctx->error_begin, ctx->error_end, ctx->input, ctx->size,
            ctx->offset);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_elemkey;
    }

    /* this should be a colon. */
    if (VCJSON_LEXER_PRIM_COLON != symbol)
    {
        retval = ERROR_VCJSON_PARSE_be519e92_b2a0_44a4_84f1_3d506fd3f54d;
        goto cleanup_elemkey;
    }

    /* read a value. */
    retval = vcjson_read_value(&elemvalue, ctx);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_elemkey;
    }

    /* put the key-value pair into the object. */
    retval = vcjson_object_put(obj, elemkey, elemvalue);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_elemvalue;
    }

    /* success. */
    retval = STATUS_SUCCESS;
    goto done;

cleanup_elemvalue:
    release_retval = resource_release(vcjson_value_resource_handle(elemvalue));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

cleanup_elemkey:
    release_retval = resource_release(vcjson_string_resource_handle(elemkey));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Create an array value from input.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param ctx           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_value_array(
        vcjson_value** value, vcjson_parser_context* ctx)
{
    status retval, release_retval;
    int symbol;
    vcjson_value* elemval;
    bool expecting_comma = false;
    slist* list;
    size_t primpos;

    /* create an slist instance for holding array values. */
    retval = slist_create(&list, ctx->alloc);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* iterate through the array. */
    for (;;)
    {
        /* peek at a primitive. */
        retval =
            vcjson_scan_primitive(
                &symbol, &primpos, ctx->input, ctx->size, ctx->offset, true);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_list;
        }

        /* decode the primitive. */
        switch (symbol)
        {
            case VCJSON_LEXER_PRIM_COMMA:
                if (expecting_comma)
                {
                    retval =
                        vcjson_scan_symbol(
                            &symbol, ctx->error_begin, ctx->error_end,
                            ctx->input, ctx->size, ctx->offset);
                    if (STATUS_SUCCESS != retval)
                    {
                        goto cleanup_list;
                    }

                    /* verify that we scanned the comma. */
                    if (VCJSON_LEXER_PRIM_COMMA != symbol)
                    {
                        retval =
                        ERROR_VCJSON_PARSE_f13a1abe_698e_4ff3_b5e7_70ac83eb1d4f;
                        goto cleanup_list;
                    }

                    expecting_comma = false;
                }
                else
                {
                    retval =
                        ERROR_VCJSON_PARSE_4b143e34_8ab5_4a34_b79c_905f66b62511;
                    goto cleanup_list;
                }
                break;

            case VCJSON_LEXER_PRIM_RIGHT_BRACKET:
                retval =
                    vcjson_scan_symbol(
                        &symbol, ctx->error_begin, ctx->error_end, ctx->input,
                        ctx->size, ctx->offset);
                if (STATUS_SUCCESS != retval)
                {
                    goto cleanup_list;
                }

                /* verify that we scanned the bracket. */
                if (VCJSON_LEXER_PRIM_RIGHT_BRACKET != symbol)
                {
                    retval =
                        ERROR_VCJSON_PARSE_f13a1abe_698e_4ff3_b5e7_70ac83eb1d4f;
                    goto cleanup_list;
                }

                /* if this is an empty array or there was no comma after the
                 * last element, then this is valid. */
                if (slist_count(list) == 0 || expecting_comma)
                {
                    /* create an array value from this list. */
                    retval =
                        vcjson_read_array_from_list(value, ctx->alloc, list);
                    if (STATUS_SUCCESS != retval)
                    {
                        goto cleanup_list;
                    }

                    /* success. */
                    retval = STATUS_SUCCESS;
                    goto cleanup_list;
                }
                else
                {
                    /* we have a hanging comma. */
                    retval =
                        ERROR_VCJSON_PARSE_e02e6452_eedc_4049_aad0_f79cbf7442a2;
                    goto cleanup_list;
                }

            default:
                if (expecting_comma)
                {
                    return
                        ERROR_VCJSON_PARSE_da3c5b50_0456_4acd_904b_2a72464e59ae;
                }
                else
                {
                    /* try to read a value from input. */
                    retval = vcjson_read_value(&elemval, ctx);
                    if (STATUS_SUCCESS != retval)
                    {
                        if (
                            ERROR_VCJSON_PARSE_RECURSION_DEPTH_EXCEEDED
                                != retval)
                        {
                        retval =
                        ERROR_VCJSON_PARSE_c207ee84_a90b_4d01_9314_a769a460819a;
                        }

                        goto cleanup_list;
                    }

                    /* append this value to the end of the list. */
                    retval = slist_append_tail(list, &elemval->hdr);
                    if (STATUS_SUCCESS != retval)
                    {
                        goto cleanup_list;
                    }

                    /* we now need a comma. */
                    expecting_comma = true;
                }
                break;
        }
    }

cleanup_list:
    release_retval = resource_release(slist_resource_handle(list));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Convert the given list to an array.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param alloc         The allocator to use for this operation.
 * \param list          The list for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
    vcjson_read_array_from_list(
        vcjson_value** value, allocator* alloc, slist* list)
{
    status retval, release_retval;
    vcjson_array* array;
    size_t size;
    size_t offset = 0;
    vcjson_value* elemval;

    /* get the size of the list. */
    size = slist_count(list);

    /* create the array instance. */
    retval = vcjson_array_create(&array, alloc, size);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* iterate over the list. */
    while (slist_count(list) > 0)
    {
        /* if somehow the offset matches the array size, exit with an error. */
        if (offset == size)
        {
            retval = ERROR_VCJSON_PARSE_a55efae1_d5fa_4c7d_ba85_f0051194a759;
            goto cleanup_array;
        }

        /* pop a value off of the list. */
        retval = slist_pop(list, (resource**)&elemval);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_array;
        }

        /* place this value into the array. */
        retval = vcjson_array_set(array, offset, elemval);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_array;
        }

        /* move to the next array slot. */
        ++offset;
    }

    /* convert the array to a value. */
    retval = vcjson_value_create_from_array(value, alloc, array);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_array;
    }

    /* success. */
    retval = STATUS_SUCCESS;
    goto done;

cleanup_array:
    release_retval = resource_release(vcjson_array_resource_handle(array));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
