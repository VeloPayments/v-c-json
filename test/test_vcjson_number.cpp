/**
 * \file test/test_vcjson_number.cpp
 *
 * \brief Unit tests for vcjson_number related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_number);

/**
 * Verify that vcjson_number_create can create a number instance.
 */
TEST(vcjson_number_create_basics)
{
    allocator* alloc = nullptr;
    vcjson_number* number = nullptr;
    const double EXPECTED_VALUE = 63.5;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a number instance. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_number_create(&number, alloc, EXPECTED_VALUE));

    /* the number value is equal to EXPECTED_VALUE. */
    TEST_EXPECT(EXPECTED_VALUE == vcjson_number_value(number));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_number_resource_handle(number)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}
