/**
 * \file test/test_vcjson_object.cpp
 *
 * \brief Unit tests for vcjson_object related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>
#include <cstring>

using namespace std;

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_object);

/**
 * Verify that vcjson_object_create can create an empty object.
 */
TEST(vcjson_object_create_basics)
{
    allocator* alloc = nullptr;
    vcjson_object* object = nullptr;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create an object instance. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_create(&object, alloc));

    /* the number of elements in this empty object is zero. */
    TEST_EXPECT(0 == vcjson_object_elements(object));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_object_resource_handle(object)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can get and put values into an object.
 */
TEST(vcjson_object_get_put)
{
    allocator* alloc = nullptr;
    vcjson_object* object = nullptr;
    vcjson_value* val = nullptr;
    const char* EXPECTED_KEY = "test key";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create an object instance. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_create(&object, alloc));

    /* the number of elements in this empty object is zero. */
    TEST_EXPECT(0 == vcjson_object_elements(object));

    /* if we get the expected key, we should get an error because the key is not
     * yet set. */
    TEST_ASSERT(
        ERROR_VCJSON_KEY_NOT_FOUND
            == vcjson_object_get(&val, object, EXPECTED_KEY));

    /* create a true value. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_value_create_from_true(&val, alloc));

    /* put the given value using the given key. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_put(object, EXPECTED_KEY, val));

    /* the value is now owned by the object. */
    val = nullptr;

    /* the get should now succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_get(&val, object, EXPECTED_KEY));

    /* the value should be our boolean value. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(val));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_object_resource_handle(object)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}
