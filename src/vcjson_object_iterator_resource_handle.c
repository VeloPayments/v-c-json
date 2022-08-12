/**
 * \file vcjson_object_iterator_resource_handle.c
 *
 * \brief Get the resource handle of an object iterator.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Get the resource handle for the given \ref vcjson_object_iterator
 * instance.
 *
 * \param iterator      The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_object_iterator_resource_handle(
    vcjson_object_iterator* iterator)
{
    return &iterator->hdr;
}
