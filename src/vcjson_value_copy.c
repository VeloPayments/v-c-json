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

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/* forward decls. */
static status vcjson_value_copy_null(
    vcjson_value** value, allocator* alloc);
static status vcjson_value_copy_object(
    vcjson_value** value, allocator* alloc, const vcjson_object* orig);
static status vcjson_value_copy_array(
    vcjson_value** value, allocator* alloc, const vcjson_array* orig);
static status vcjson_value_copy_number(
    vcjson_value** value, allocator* alloc, const vcjson_number* orig);
static status vcjson_value_copy_string(
    vcjson_value** value, allocator* alloc, const vcjson_string* orig);
static status vcjson_value_copy_bool(
    vcjson_value** value, allocator* alloc, const vcjson_bool* orig);

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
    /* decode the value type. */
    switch (orig->type)
    {
        /* copy a null value. */
        case VCJSON_VALUE_TYPE_NULL:
            return vcjson_value_copy_null(value, alloc);

        /* copy an object value. */
        case VCJSON_VALUE_TYPE_OBJECT:
            return
                vcjson_value_copy_object(
                    value, alloc, (const vcjson_object*)orig->value);

        /* copy an array value. */
        case VCJSON_VALUE_TYPE_ARRAY:
            return
                vcjson_value_copy_array(
                    value, alloc, (const vcjson_array*)orig->value);

        /* copy a number value. */
        case VCJSON_VALUE_TYPE_NUMBER:
            return
                vcjson_value_copy_number(
                    value, alloc, (const vcjson_number*)orig->value);

        /* copy a string value. */
        case VCJSON_VALUE_TYPE_STRING:
            return
                vcjson_value_copy_string(
                    value, alloc, (const vcjson_string*)orig->value);

        /* copy a bool value. */
        case VCJSON_VALUE_TYPE_BOOL:
            return
                vcjson_value_copy_bool(
                    value, alloc, (const vcjson_bool*)orig->value);

        /* an unknown value type was encountered... */
        default:
            return ERROR_VCJSON_INVALID_GET;
    }
}

/**
 * \brief Copy a null value.
 */
static status vcjson_value_copy_null(
    vcjson_value** value, allocator* alloc)
{
    return vcjson_value_create_from_null(value, alloc);
}

/**
 * \brief Copy an object value.
 */
static status vcjson_value_copy_object(
    vcjson_value** value, allocator* alloc, const vcjson_object* orig)
{
    status retval, release_retval;
    vcjson_object* tmp;

    /* copy the object. */
    retval = vcjson_object_copy(&tmp, alloc, orig);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* wrap this object in a new value. */
    retval = vcjson_value_create_from_object(value, alloc, tmp);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. */
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
 * \brief Copy an array value.
 */
static status vcjson_value_copy_array(
    vcjson_value** value, allocator* alloc, const vcjson_array* orig)
{
    status retval, release_retval;
    vcjson_array* tmp;

    /* copy the array. */
    retval = vcjson_array_copy(&tmp, alloc, orig);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* wrap this array in a new value. */
    retval = vcjson_value_create_from_array(value, alloc, tmp);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. */
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
 * \brief Copy a number value.
 */
static status vcjson_value_copy_number(
    vcjson_value** value, allocator* alloc, const vcjson_number* orig)
{
    status retval, release_retval;
    vcjson_number* tmp;

    /* copy the number. */
    retval = vcjson_number_copy(&tmp, alloc, orig);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* wrap this number in a new value. */
    retval = vcjson_value_create_from_number(value, alloc, tmp);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. */
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
 * \brief Copy a string value.
 */
static status vcjson_value_copy_string(
    vcjson_value** value, allocator* alloc, const vcjson_string* orig)
{
    status retval, release_retval;
    vcjson_string* tmp;

    /* copy the string. */
    retval = vcjson_string_copy(&tmp, alloc, orig);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* wrap this string in a new value. */
    retval = vcjson_value_create_from_string(value, alloc, tmp);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. */
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
 * \brief Copy a bool value.
 */
static status vcjson_value_copy_bool(
    vcjson_value** value, allocator* alloc, const vcjson_bool* orig)
{
    /* handle true. */
    if (orig == VCJSON_TRUE)
    {
        return vcjson_value_create_from_true(value, alloc);
    }
    /* otherwise, handle false. */
    else
    {
        return vcjson_value_create_from_false(value, alloc);
    }
}
