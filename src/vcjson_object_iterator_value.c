/**
 * \file vcjson_object_iterator_value.c
 *
 * \brief Get the key and value at the current object iterator position.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_rbtree;

/**
 * \brief Get the key-value pair associated with the current iterator position.
 *
 * \note The key and value are both owned by the object and cannot be modified.
 * Furthermore, these values should not be used if the underlying object is
 * modified, as these pointers will no longer be valid. If the iterator is
 * invalid (an error was returned when it was iterated or created), then it is
 * an error to use this operation and the result is undefined and unsafe.
 *
 * \param key           Pointer to receive the key value at this location.
 * \param value         Pointer to receive the value pointer at this location.
 * \param iterator      The iterator for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_ITERATOR_BAD if the iterator is bad.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_value(
    const vcjson_string** key, vcjson_value** value,
    vcjson_object_iterator* iterator)
{
    vcjson_object_element* elem = NULL;

    /* Exit if the iterator is not currently valid. */
    if (iterator->curr == iterator->nil)
    {
        return ERROR_VCJSON_ITERATOR_BAD;
    }

    /* get the element at this iterator position. */
    elem = (vcjson_object_element*)
            rbtree_node_value(
                iterator->tree, iterator->curr);
    if (NULL == elem)
    {
        return ERROR_VCJSON_ITERATOR_BAD;
    }

    /* Success. Set the key and value. */
    *key = elem->key;
    *value = elem->value;
    return STATUS_SUCCESS;
}
