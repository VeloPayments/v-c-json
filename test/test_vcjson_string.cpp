/**
 * \file test/test_vcjson_string.cpp
 *
 * \brief Unit tests for vcjson_string related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <string.h>
#include <vcjson/vcjson.h>

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_string);

/**
 * Verify that vcjson_string_create can create a string instance.
 */
TEST(vcjson_string_create_basics)
{
    allocator* alloc = nullptr;
    vcjson_string* string = nullptr;
    const char* EXPECTED_VALUE = "This is a test.";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a string instance. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_string_create(&string, alloc, EXPECTED_VALUE));

    /* the string value is equal to EXPECTED_VALUE. */
    TEST_EXPECT(0 == strcmp(EXPECTED_VALUE, vcjson_string_value(string)));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_string_resource_handle(string)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}
