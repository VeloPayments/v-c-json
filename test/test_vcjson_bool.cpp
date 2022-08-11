/**
 * \file test/test_vcjson_bool.cpp
 *
 * \brief Unit tests for vcjson_bool related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_bool);

/**
 * \brief Verify that VCJSON_TRUE and VCJSON_FALSE make sense...
 */
TEST(basics)
{
    /* coercing a vcjson_bool resource to release it has no meaning. */
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_TRUE));
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_TRUE));
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_TRUE));

    /* coercing a vcjson_bool resource to release it has no meaning. */
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_FALSE));
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_FALSE));
    TEST_EXPECT(STATUS_SUCCESS == resource_release((resource*)VCJSON_FALSE));

    /* TRUE and FALSE are not the same. */
    TEST_EXPECT(VCJSON_TRUE != VCJSON_FALSE);
}
