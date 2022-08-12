/**
 * \file vcjson_object_get.c
 *
 * \brief Get a value from an object by key.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_rbtree;
RCPR_IMPORT_resource;

/**
 * \brief Get a value from the object using the given key.
 *
 * On success, the value pointer is updated with the \ref vcjson_value instance
 * associated with the given key in this object.
 *
 * \note Ownership of the returned value remains with the object.
 *
 * \param value         Pointer to the value pointer to be updated on success.
 * \param obj           The object instance for this operation.
 * \param key           The key for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_KEY_NOT_FOUND if the key is not associated with a value.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_get(
    vcjson_value** value, vcjson_object* obj, const char* key)
{
    status retval;
    vcjson_object_element* elem;

    /* attempt to find a value by key in the object's rbtree. */
    retval = rbtree_find((resource**)&elem, obj->elements, key);
    if (ERROR_RBTREE_NOT_FOUND == retval)
    {
        return ERROR_VCJSON_KEY_NOT_FOUND;
    }
    else if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* set the value. */
    *value = elem->value;
    return STATUS_SUCCESS;
}
