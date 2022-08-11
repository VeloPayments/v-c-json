/**
 * \file test/test_vcjson_null.cpp
 *
 * \brief Unit tests for vcjson_null related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_null);

/**
 * \brief Verify that VCJSON_NULL makes sense...
 */
TEST(basics)
{
    /* coercing a vcjson_null resource to release it has no meaning. */
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_NULL));
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_NULL));
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_NULL));
}
