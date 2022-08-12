/**
 * \file vcjson_object_element_compare.c
 *
 * \brief Compare two object element keys.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Comparison function for an object elements tree.
 *
 * \param context       Unused.
 * \param lhs           The left-hand-side key to compare.
 * \param rhs           The right-hand-side key to compare.
 *
 * \returns an integer value representing the comparison result.
 *      - RCPR_COMPARE_LT if \p lhs &lt; \p rhs.
 *      - RCPR_COMPARE_EQ if \p lhs == \p rhs.
 *      - RCPR_COMPARE_GT if \p lhs &gt; \p rhs.
 */
RCPR_SYM(rcpr_comparison_result) vcjson_object_element_compare(
    void* /*context*/, const void* lhs, const void* rhs)
{
    const char* l = (const char*)lhs;
    const char* r = (const char*)rhs;

    int res = strcmp(l, r);
    if (res < 0)
    {
        return RCPR_COMPARE_LT;
    }
    else if (res > 0)
    {
        return RCPR_COMPARE_GT;
    }
    else
    {
        return RCPR_COMPARE_EQ;
    }
}
