/**
 * \file vcjson_object_element_key.c
 *
 * \brief Get the key for an object element.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Given an object element resource, return the key for this resource.
 *
 * \param context       Unused.
 * \param r             The resource.
 *
 * \returns the key for this resource.
 */
const void* vcjson_object_element_key(
    void* /*context*/, const RCPR_SYM(resource)* r)
{
    const vcjson_object_element* elem = (const vcjson_object_element*)r;

    return elem->key;
}
