/**
 * \file test/test_vcjson_value.cpp
 *
 * \brief Unit tests for vcjson_value related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_value);

/**
 * \brief Test for null related values.
 */
TEST(value_null_basics)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a NULL value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_create_from_null(&value, alloc));

    /* the type of this value is VCJSON_VALUE_TYPE_NULL. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_NULL == vcjson_value_type(value));

    /* clean up. */
    resource_release(vcjson_value_resource_handle(value)); /* does nothing. */
    resource_release(allocator_resource_handle(alloc));
}

/**
 * \brief Test for true related values.
 */
TEST(value_true_basics)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_bool* boolval = nullptr;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a true value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_create_from_true(&value, alloc));

    /* the type of this value is VCJSON_VALUE_TYPE_BOOL. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(value));

    /* get the bool value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_bool(&boolval, value));

    /* this bool value is TRUE. */
    TEST_EXPECT(VCJSON_TRUE == boolval);

    /* clean up. */
    resource_release(vcjson_value_resource_handle(value)); /* does nothing. */
    resource_release(allocator_resource_handle(alloc));
}

/**
 * \brief Test for false related values.
 */
TEST(value_false_basics)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_bool* boolval = nullptr;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a true value. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_value_create_from_false(&value, alloc));

    /* the type of this value is VCJSON_VALUE_TYPE_BOOL. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(value));

    /* get the bool value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_bool(&boolval, value));

    /* this bool value is FALSE. */
    TEST_EXPECT(VCJSON_FALSE == boolval);

    /* clean up. */
    resource_release(vcjson_value_resource_handle(value)); /* does nothing. */
    resource_release(allocator_resource_handle(alloc));
}
