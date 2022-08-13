/**
 * \file vcjson_object_copy.c
 *
 * \brief Copy an object instance.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_rbtree;
RCPR_IMPORT_resource;

/**
 * \brief Make a deep copy of the given object.
 *
 * \note On success, this function creates a  \ref vcjson_object instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param obj           Pointer to the object pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original object to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_copy(
    vcjson_object** obj, RCPR_SYM(allocator)* alloc, const vcjson_object* orig)
{
    status retval, release_retval;
    rbtree_node* nil;
    rbtree_node* root;
    rbtree_node* iter;
    vcjson_object_element* elem;
    vcjson_value* value;
    vcjson_object* tmp;

    /* create an empty object instance to hold this copy. */
    retval = vcjson_object_create(&tmp, alloc);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* get the nil node for the original object tree. */
    nil = rbtree_nil_node(orig->elements);

    /* get the root node for the original object tree. */
    root = rbtree_root_node(orig->elements);

    /* get the first node for this object tree. */
    iter = rbtree_minimum_node(orig->elements, root);

    /* iterate through this object tree. */
    while (nil != iter)
    {
        /* get the element for this node. */
        elem = (vcjson_object_element*)rbtree_node_value(orig->elements, iter);

        /* duplicate the value of this object. */
        retval = vcjson_value_copy(&value, alloc, elem->value);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_tmp;
        }

        /* put this copy into the cloned object. */
        retval = vcjson_object_put(tmp, elem->key, value);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_value;
        }

        /* get the next element in this object tree. */
        iter = rbtree_successor_node(orig->elements, iter);
    }

    /* Success. Recursive deep copy complete. */
    *obj = tmp;
    retval = STATUS_SUCCESS;
    goto done;

cleanup_value:
    release_retval = resource_release(&value->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
