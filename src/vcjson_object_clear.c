/**
 * \file vcjson_object_clear.c
 *
 * \brief Clear an object.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_rbtree;

/**
 * \brief Clear the given object of all key-value pairs.
 *
 * \note Any key-value pair associated with this object will be released using
 * \ref resource_release. When this operation completes successfully, the object
 * is reset to the same state it was in when first created.
 *
 * \param obj           The object instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_clear(vcjson_object* obj)
{
    return
        rbtree_clear(obj->elements);
}
