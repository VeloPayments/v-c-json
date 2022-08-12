/**
 * \file vcjson_object_elements.c
 *
 * \brief Return the number of elements in this object.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_rbtree;

/**
 * \brief Get the number of elements in the given object instance.
 *
 * \param obj           The array instance for this operation.
 *
 * \returns the number of elements in this object instance.
 */
size_t vcjson_object_elements(const vcjson_object* obj)
{
    return rbtree_count(obj->elements);
}
