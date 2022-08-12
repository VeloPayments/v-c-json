/**
 * \file vcjson_object_iterator_create.c
 *
 * \brief Create an object iterator instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_rbtree;
RCPR_IMPORT_resource;

/**
 * \brief Create an object iterator set to the first key-value pair in this
 * object instance.
 *
 * On success, the given object iterator pointer is set to the first key-value
 * pair in this object instance.
 *
 * \note This iterator is only valid as long as the object instance is not
 * modified. If the object instance is modified, using this iterator will cause
 * undefined behavior. This iterator is a resource and must be released using
 * \ref resource_release when no longer needed.
 *
 * \param iterator      Pointer to the iterator pointer to be updated with a new
 *                      iterator instance.
 * \param obj           The object instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_ITERATOR_END if there are no key-value pairs in this
 *        object instance.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_create(
    vcjson_object_iterator** iterator, vcjson_object* obj)
{
    status retval;
    vcjson_object_iterator* tmp = NULL;

    /* allocate memory for this iterator instance. */
    retval = allocator_allocate(obj->alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear this memory. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize resource. */
    resource_init(&tmp->hdr, &vcjson_object_iterator_resource_release);

    /* set initial values. */
    tmp->alloc = obj->alloc;
    tmp->tree = obj->elements;
    tmp->nil = rbtree_nil_node(obj->elements);
    tmp->curr =
        rbtree_minimum_node(obj->elements, rbtree_root_node(obj->elements));

    /* success. */
    retval = STATUS_SUCCESS;
    *iterator = tmp;
    goto done;

done:
    return retval;
}
