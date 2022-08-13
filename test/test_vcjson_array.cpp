/**
 * \file test/test_vcjson_array.cpp
 *
 * \brief Unit tests for vcjson_array related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_array);

/**
 * Verify that vcjson_array_create can create an array of the given size.
 */
TEST(vcjson_array_create_basics)
{
    allocator* alloc = nullptr;
    vcjson_array* array = nullptr;
    vcjson_value* value = nullptr;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create an array instance. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_array_create(&array, alloc, 3));

    /* the size of this array is 3. */
    TEST_EXPECT(3 == vcjson_array_size(array));

    /* the type of the 0th element is null. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_array_get(&value, array, 0));
    TEST_EXPECT(VCJSON_VALUE_TYPE_NULL == vcjson_value_type(value));

    /* the type of the 1st element is null. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_array_get(&value, array, 1));
    TEST_EXPECT(VCJSON_VALUE_TYPE_NULL == vcjson_value_type(value));

    /* the type of the 2nd element is null. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_array_get(&value, array, 2));
    TEST_EXPECT(VCJSON_VALUE_TYPE_NULL == vcjson_value_type(value));

    /* attempting to get the 3rd element will cause an out-of-bounds error. */
    TEST_ASSERT(
        ERROR_VCJSON_ARRAY_INDEX_OUT_OF_BOUNDS
            == vcjson_array_get(&value, array, 3));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_array_resource_handle(array)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}
