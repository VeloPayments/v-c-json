/**
 * \file vcjson_emit_string.c
 *
 * \brief Emit a JSON value as a string.
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
static status vcjson_emit_to_counter(
    void* context, const void* val, size_t size);
static status vcjson_emit_to_string(
    void* context, const void* val, size_t size);
static status vcjson_emit_value(
    vcjson_emit_fn emitter, void* context, vcjson_value* value);
static status vcjson_emit_value_bool(
    vcjson_emit_fn emitter, void* context, vcjson_value* value);
static status vcjson_emit_value_number(
    vcjson_emit_fn emitter, void* context, vcjson_value* value);
static status vcjson_emit_value_string(
    vcjson_emit_fn emitter, void* context, vcjson_value* value);
static status vcjson_emit_decoded_string(
    vcjson_emit_fn emitter, void* context, vcjson_string* stringval);
static status vcjson_emit_value_string_simple_escape(
    vcjson_emit_fn emitter, void* context, char escape);
static status vcjson_emit_value_object(
    vcjson_emit_fn emitter, void* context, vcjson_value* value);
static status vcjson_emit_value_array(
    vcjson_emit_fn emitter, void* context, vcjson_value* value);

typedef struct vcjson_emit_string_context vcjson_emit_string_context;
struct vcjson_emit_string_context
{
    char* outbuf;
    size_t offset;
    size_t maxlen;
};

/**
 * \brief Emit a JSON value as a string.
 *
 * \note On success, this function creates a string that is owned by the caller.
 * This string is a resource that must be released when no longer in use.
 * 
 * \param string        Pointer to the string pointer to hold the JSON string.
 * \param alloc         The allocator to use for this operation.
 * \param value         The JSON value to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_emit_string(
    vcjson_string** string, RCPR_SYM(allocator)* alloc, vcjson_value* value)
{
    status retval, release_retval;
    size_t size = 0;
    vcjson_emit_string_context ctx;
    vcjson_string* tmp;

    /* get the size of the emitted value. */
    retval = vcjson_emit_value(&vcjson_emit_to_counter, &size, value);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* allocate memory for the string instance. */
    retval = allocator_allocate(alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear structure. */
    memset(tmp, 0, sizeof(*tmp));

    /* init resource. */
    resource_init(&tmp->hdr, &vcjson_string_resource_release);

    /* set the string allocator. */
    tmp->alloc = alloc;

    /* allocate memory for the string buffer. */
    retval = allocator_allocate(alloc, (void**)&tmp->value, size + 1);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* clear string buffer. */
    memset(tmp->value, 0, size + 1);

    /* set the string length. */
    tmp->length = size;

    /* set up the string emitter context. */
    ctx.outbuf = tmp->value;
    ctx.offset = 0;
    ctx.maxlen = size;

    /* emit the value to the string. */
    retval = vcjson_emit_value(&vcjson_emit_to_string, &ctx, value);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. Set string. */
    *string = tmp;
    retval = STATUS_SUCCESS;
    goto done;

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Accumulate writes to a total size.
 *
 * \param context       Opaque pointer to the counter for this operation.
 * \param val           The buffer to write.
 * \param size          The size to write.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_to_counter(
    void* context, const void* /*val*/, size_t size)
{
    size_t* count = (size_t*)context;

    *count += size;

    return STATUS_SUCCESS;
}

/**
 * \brief Write to the output buffer.
 *
 * \param context       Opaque pointer to the output buffer.
 * \param val           The buffer to write.
 * \param size          The size to write.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_to_string(
    void* context, const void* val, size_t size)
{
    vcjson_emit_string_context* ctx = (vcjson_emit_string_context*)context;

    /* verify that we can write to this buffer. */
    if (ctx->offset + size > ctx->maxlen)
    {
        return ERROR_VCJSON_EMIT_BUFFER_OVERRUN;
    }

    /* write the data to the buffer. */
    memcpy(ctx->outbuf + ctx->offset, val, size);

    /* increment offset. */
    ctx->offset += size;

    /* success. */
    return STATUS_SUCCESS;
}

/**
 * \brief Emit a JSON value using the given emitter.
 *
 * \param emitter       Pointer to the function to use to emit data.
 * \param context       The user context to pass to the emitter.
 * \param value         The JSON value to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_value(
    vcjson_emit_fn emitter, void* context, vcjson_value* value)
{
    switch (vcjson_value_type(value))
    {
        case VCJSON_VALUE_TYPE_NULL:
            return emitter(context, "null", 4);

        case VCJSON_VALUE_TYPE_BOOL:
            return vcjson_emit_value_bool(emitter, context, value);

        case VCJSON_VALUE_TYPE_NUMBER:
            return vcjson_emit_value_number(emitter, context, value);

        case VCJSON_VALUE_TYPE_STRING:
            return vcjson_emit_value_string(emitter, context, value);

        case VCJSON_VALUE_TYPE_OBJECT:
            return vcjson_emit_value_object(emitter, context, value);

        case VCJSON_VALUE_TYPE_ARRAY:
            return vcjson_emit_value_array(emitter, context, value);

        default:
            return ERROR_VCJSON_EMIT_UNKNOWN_VALUE_TYPE;
    }
}

/**
 * \brief Emit a JSON boolean using the given emitter.
 *
 * \param emitter       Pointer to the function to use to emit data.
 * \param context       The user context to pass to the emitter.
 * \param value         The JSON value to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_value_bool(
    vcjson_emit_fn emitter, void* context, vcjson_value* value)
{
    status retval;
    vcjson_bool* boolval;

    /* get the boolean value. */
    retval = vcjson_value_get_bool(&boolval, value);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* if the value is true, emit true. */
    if (VCJSON_TRUE == boolval)
    {
        return emitter(context, "true", 4);
    }
    /* otherwise, emit false. */
    else
    {
        return emitter(context, "false", 5);
    }
}

/**
 * \brief Emit a JSON number using the given emitter.
 *
 * \param emitter       Pointer to the function to use to emit data.
 * \param context       The user context to pass to the emitter.
 * \param value         The JSON value to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_value_number(
    vcjson_emit_fn emitter, void* context, vcjson_value* value)
{
    status retval;
    vcjson_number* numberval;
    char buffer[1024];
    size_t buffersize = sizeof(buffer);
    int maxsize;

    /* get the number value. */
    retval = vcjson_value_get_number(&numberval, value);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* format the number. */
    maxsize =
        snprintf(buffer, buffersize, "%f", vcjson_number_value(numberval));
    if (maxsize < 0)
    {
        retval = ERROR_VCJSON_EMIT_NUMBER_FORMAT;
        goto cleanup_buffer;
    }
    else if ((size_t)maxsize > buffersize)
    {
        maxsize = buffersize-1;
    }

    /* emit the value. */
    retval = emitter(context, buffer, maxsize);
    goto cleanup_buffer;

cleanup_buffer:
    memset(buffer, 0, sizeof(buffer));

done:
    return retval;
}

/**
 * \brief Emit a JSON string using the given emitter.
 *
 * \param emitter       Pointer to the function to use to emit data.
 * \param context       The user context to pass to the emitter.
 * \param value         The JSON value to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_value_string(
    vcjson_emit_fn emitter, void* context, vcjson_value* value)
{
    status retval;
    vcjson_string* stringval;

    /* get the string value. */
    retval = vcjson_value_get_string(&stringval, value);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* emit the decoded string value. */
    return vcjson_emit_decoded_string(emitter, context, stringval);
}

/**
 * \brief Emit a decoded JSON string using the given emitter.
 *
 * \param emitter       Pointer to the function to use to emit data.
 * \param context       The user context to pass to the emitter.
 * \param stringval     The JSON string to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_decoded_string(
    vcjson_emit_fn emitter, void* context, vcjson_string* stringval)
{
    status retval;
    const char* str;
    size_t length;

    /* get the string. */
    str = vcjson_string_value(stringval, &length);

    /* emit the open quote. */
    retval = emitter(context, "\"", 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* iterate over all string bytes. */
    for (size_t i = 0; i < length; ++i)
    {
        switch (str[i])
        {
            case '\b':
                /* emit the escape. */
                retval =
                    vcjson_emit_value_string_simple_escape(
                        emitter, context, 'b');
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;

            case '\f':
                /* emit the escape. */
                retval =
                    vcjson_emit_value_string_simple_escape(
                        emitter, context, 'f');
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;

            case '\n':
                /* emit the escape. */
                retval =
                    vcjson_emit_value_string_simple_escape(
                        emitter, context, 'n');
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;

            case '\r':
                /* emit the escape. */
                retval =
                    vcjson_emit_value_string_simple_escape(
                        emitter, context, 'r');
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;

            case '\t':
                /* emit the escape. */
                retval =
                    vcjson_emit_value_string_simple_escape(
                        emitter, context, 't');
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;

            case '\\':
                /* emit the escape. */
                retval =
                    vcjson_emit_value_string_simple_escape(
                        emitter, context, '\\');
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;

            case '/':
                /* emit the escape. */
                retval =
                    vcjson_emit_value_string_simple_escape(
                        emitter, context, '/');
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;

            case '"':
                /* emit the escape. */
                retval =
                    vcjson_emit_value_string_simple_escape(
                        emitter, context, '"');
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;

            /* TODO - handle other control chars once \u support is added. */

            /* all other bytes can be emitted directly. */
            default:
                retval = emitter(context, str + i, 1);
                if (STATUS_SUCCESS != retval)
                {
                    goto done;
                }
                break;
        }
    }

    /* emit the close quote. */
    retval = emitter(context, "\"", 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

done:
    return retval;
}

/**
 * \brief Emit a simple escape sequence using the given emitter.
 *
 * \param emitter       Pointer to the function to use to emit data.
 * \param context       The user context to pass to the emitter.
 * \param escape        The escape to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_value_string_simple_escape(
    vcjson_emit_fn emitter, void* context, char escape)
{
    status retval;
    char buffer[1024];
    int fmt_len;

    /* format the escape. */
    fmt_len = snprintf(buffer, sizeof(buffer), "\\%c", escape);
    if (fmt_len < 0)
    {
        retval = ERROR_VCJSON_EMIT_GENERAL_FORMAT;
        goto cleanup_buffer;
    }

    /* emit the escape. */
    retval = emitter(context, buffer, (size_t)fmt_len);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_buffer;
    }

cleanup_buffer:
    memset(buffer, 0, sizeof(buffer));

    return retval;
}

/**
 * \brief Emit a JSON object using the given emitter.
 *
 * \param emitter       Pointer to the function to use to emit data.
 * \param context       The user context to pass to the emitter.
 * \param value         The value to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_value_object(
    vcjson_emit_fn emitter, void* context, vcjson_value* value)
{
    status retval;
    vcjson_object* objval;
    vcjson_object_iterator* iter;
    const vcjson_string* key;
    vcjson_value* val;
    bool emit_comma = false;

    /* get the object value. */
    retval = vcjson_value_get_object(&objval, value);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* emit the open brace. */
    retval = emitter(context, "{", 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* create an iterator for iterating over the object. */
    retval = vcjson_object_iterator_create(&iter, objval);
    if (STATUS_SUCCESS != retval && ERROR_VCJSON_ITERATOR_END != retval)
    {
        goto done;
    }

    /* iterate through all members. */
    while (ERROR_VCJSON_ITERATOR_END != retval)
    {
        /* should we emit a comma? */
        if (emit_comma)
        {
            retval = emitter(context, ",", 1);
            if (STATUS_SUCCESS != retval)
            {
                goto done;
            }
        }

        /* get the key and value. */
        retval = vcjson_object_iterator_value(&key, &val, iter);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* emit the key string. */
        retval =
            vcjson_emit_decoded_string(emitter, context, (vcjson_string*)key);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* emit the colon. */
        retval = emitter(context, ":", 1);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* emit the value. */
        retval = vcjson_emit_value(emitter, context, val);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* emit a comma for each subsequent member. */
        emit_comma = true;

        /* get the next iterator value. */
        retval = vcjson_object_iterator_next(iter);
    }

    /* emit the close brace. */
    retval = emitter(context, "}", 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

done:
    return retval;
}

/**
 * \brief Emit a JSON array using the given emitter.
 *
 * \param emitter       Pointer to the function to use to emit data.
 * \param context       The user context to pass to the emitter.
 * \param value         The value to emit.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_emit_value_array(
    vcjson_emit_fn emitter, void* context, vcjson_value* value)
{
    status retval;
    vcjson_array* arrayval;
    vcjson_value* val;
    size_t elements;
    bool emit_comma = false;

    /* get the array value. */
    retval = vcjson_value_get_array(&arrayval, value);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* emit the open bracket. */
    retval = emitter(context, "[", 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* get the number of array elements. */
    elements = vcjson_array_size(arrayval);

    /* iterate through all array elements. */
    for (size_t i = 0; i < elements; ++i)
    {
        /* should we emit a comma? */
        if (emit_comma)
        {
            retval = emitter(context, ",", 1);
            if (STATUS_SUCCESS != retval)
            {
                goto done;
            }
        }

        /* get the array value at this offset. */
        retval = vcjson_array_get(&val, arrayval, i);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* emit the value. */
        retval = vcjson_emit_value(emitter, context, val);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* we need to emit a comma for all subsequent elements. */
        emit_comma = true;
    }

    /* emit the close bracket. */
    retval = emitter(context, "]", 1);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

done:
    return retval;
}
