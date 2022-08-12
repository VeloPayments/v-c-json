/**
 * \file vcjson_object_iterator_next.c
 *
 * \brief Increment the object iterator to the next key-value pair.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

RCPR_IMPORT_rbtree;

/**
 * \brief Increment the given iterator to the next key-value pair in the object
 * to which it points.
 *
 * \note This iterator is only valid as long as the object instance is not
 * modified. If the object instance is modified, using this iterator will cause
 * undefined behavior.
 *
 * \param iterator      The iterator to increment.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_ITERATOR_END if the iterator has been incremented to the
 *        end of the object instance.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_next(vcjson_object_iterator* iterator)
{
    /* verify that this iterator is valid. */
    if (iterator->curr == iterator->nil)
    {
        return ERROR_VCJSON_ITERATOR_END;
    }

    /* get the next node. */
    iterator->curr = rbtree_successor_node(iterator->tree, iterator->curr);
    if (iterator->curr == iterator->nil)
    {
        return ERROR_VCJSON_ITERATOR_END;
    }

    /* success. */
    return STATUS_SUCCESS;
}
