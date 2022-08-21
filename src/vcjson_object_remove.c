/**
 * \file vcjson_object_remove.c
 *
 * \brief Remove an element in the given object by key.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_rbtree;

/**
 * \brief Remove the given key from the object.
 *
 * \note This operation releases the resource relating to any value associated
 * with the given key. This operation will fail with \ref
 * VCJSON_ERROR_KEY_NOT_FOUND if this key is not found in the object. This
 * failure case should be checked if the caller does not know whether the given
 * key exists in the object.
 *
 * \param obj           The object instance for this operation.
 * \param key           The key for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_remove(vcjson_object* obj, const vcjson_string* key)
{
    status retval;

    /* delete this key from the tree. */
    retval = rbtree_delete(NULL, obj->elements, key);
    if (ERROR_RBTREE_NOT_FOUND == retval)
    {
        /* if the element does not exist, then it is removed... */
        return STATUS_SUCCESS;
    }
    else if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* success. */
    return STATUS_SUCCESS;
}
