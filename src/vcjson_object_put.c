/**
 * \file vcjson_object_put.c
 *
 * \brief Put a value into an object by key.
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
 * \brief Put a value into the given \ref vcjson_object instance.
 *
 * \note On success, the \ref vcjson_object assumes ownership of the key and
 * value. If there is already a value associated with the given key,
 * its resource is released via \ref resource_release, as is the provided key's
 * resource.
 *
 * \param obj           The object instance for this operation.
 * \param key           The key for this operation.
 * \param value         The value to insert into this object.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_put(
    vcjson_object* obj, vcjson_string* key, vcjson_value* value)
{
    status retval, release_retval;
    vcjson_object_element* elem;

    /* attempt to find a value by key in the object's rbtree. */
    retval = rbtree_find((resource**)&elem, obj->elements, key);
    if (ERROR_RBTREE_NOT_FOUND == retval)
    {
        /* allocate the element if it's not found. */
        retval = allocator_allocate(obj->alloc, (void**)&elem, sizeof(*elem));
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* clear memory. */
        memset(elem, 0, sizeof(*elem));

        /* initialize resource. */
        resource_init(&elem->hdr, &vcjson_object_element_resource_release);

        /* set initial values. */
        elem->alloc = obj->alloc;
        elem->owns_key = false;
        elem->key = key;

        /* insert this key into the tree. */
        retval = rbtree_insert(obj->elements, &elem->hdr);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_elem;
        }
    }
    else if (STATUS_SUCCESS == retval)
    {
        /* release the old value if it's found. */
        retval = resource_release(&elem->value->hdr);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* release the old key if it's found. */
        retval = resource_release(&elem->key->hdr);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* set the new key. */
        elem->key = key;
    }
    else
    {
        goto done;
    }

    /* set the element value. */
    elem->value = value;
    elem->owns_key = true;

    /* success. */
    retval = STATUS_SUCCESS;
    goto done;

cleanup_elem:
    release_retval = resource_release(&elem->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
