/**
 * \file test/test_vcjson_copy.cpp
 *
 * \brief Unit tests for vcjson copy related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>
#include <cstring>

using namespace std;

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_copy);

/**
 * Test that we can copy a number.
 */
TEST(copy_number)
{
    allocator* alloc = nullptr;
    vcjson_number* numberval = nullptr;
    vcjson_number* copyval = nullptr;
    const double EXPECTED_VALUE = 99.25;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a number. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_number_create(&numberval, alloc, EXPECTED_VALUE));

    /* copy the number. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_number_copy(&copyval, alloc, numberval));

    /* the copy should not be the same pointer as the original. */
    TEST_EXPECT(numberval != copyval);

    /* the copy's value should be our expected value. */
    TEST_EXPECT(EXPECTED_VALUE == vcjson_number_value(copyval));

    /* clean up. */
    resource_release(vcjson_number_resource_handle(numberval));
    resource_release(vcjson_number_resource_handle(copyval));
    resource_release(allocator_resource_handle(alloc));
}

/**
 * Test that we can copy a string.
 */
TEST(copy_string)
{
    allocator* alloc = nullptr;
    vcjson_string* stringval = nullptr;
    vcjson_string* copyval = nullptr;
    const char* EXPECTED_VALUE = "This is a test.";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_string_create(&stringval, alloc, EXPECTED_VALUE));

    /* copy the string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_string_copy(&copyval, alloc, stringval));

    /* the copy should not be the same pointer as the original. */
    TEST_EXPECT(stringval != copyval);

    /* the copy's value should be our expected value. */
    TEST_EXPECT(0 == strcmp(EXPECTED_VALUE, vcjson_string_value(copyval)));

    /* clean up. */
    resource_release(vcjson_string_resource_handle(stringval));
    resource_release(vcjson_string_resource_handle(copyval));
    resource_release(allocator_resource_handle(alloc));
}
