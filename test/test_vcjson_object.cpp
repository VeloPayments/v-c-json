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

    /* the number of elements in this object is now one. */
    TEST_EXPECT(1 == vcjson_object_elements(object));

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

/**
 * Verify that we can replace an object.
 */
TEST(vcjson_object_get_put_replace)
{
    allocator* alloc = nullptr;
    vcjson_object* object = nullptr;
    vcjson_value* val = nullptr;
    vcjson_bool* boolval = nullptr;
    const char* EXPECTED_KEY = "test key";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create an object instance. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_create(&object, alloc));

    /* create a true value. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_value_create_from_true(&val, alloc));

    /* put the given value using the given key. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_put(object, EXPECTED_KEY, val));

    /* the value is now owned by the object. */
    val = nullptr;

    /* create a false value. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_value_create_from_false(&val, alloc));

    /* put the given value using the given key. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_put(object, EXPECTED_KEY, val));

    /* the value is now owned by the object. */
    val = nullptr;

    /* the get should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_get(&val, object, EXPECTED_KEY));

    /* get the boolean value. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_value_get_bool(&boolval, val));

    /* this value is FALSE. */
    TEST_EXPECT(VCJSON_FALSE == boolval);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_object_resource_handle(object)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can remove a key from an object.
 */
TEST(vcjson_object_remove)
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

    /* create a true value. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_value_create_from_true(&val, alloc));

    /* put the given value using the given key. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_put(object, EXPECTED_KEY, val));

    /* the value is now owned by the object. */
    val = nullptr;

    /* the number of elements in this object is now one. */
    TEST_EXPECT(1 == vcjson_object_elements(object));

    /* remove this key. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_object_remove(object, EXPECTED_KEY));

    /* the number of elements in this object is now zero. */
    TEST_EXPECT(0 == vcjson_object_elements(object));

    /* the get should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_KEY_NOT_FOUND
            == vcjson_object_get(&val, object, EXPECTED_KEY));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_object_resource_handle(object)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can clear an object.
 */
TEST(vcjson_object_clear)
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

    /* create a true value. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_value_create_from_true(&val, alloc));

    /* put the given value using the given key. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_put(object, EXPECTED_KEY, val));

    /* the value is now owned by the object. */
    val = nullptr;

    /* the number of elements in this object is now one. */
    TEST_EXPECT(1 == vcjson_object_elements(object));

    /* clear the object. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_object_clear(object));

    /* the number of elements in this object is now zero. */
    TEST_EXPECT(0 == vcjson_object_elements(object));

    /* the get should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_KEY_NOT_FOUND
            == vcjson_object_get(&val, object, EXPECTED_KEY));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_object_resource_handle(object)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can iterate over an object.
 */
TEST(vcjson_object_iterate)
{
    allocator* alloc = nullptr;
    vcjson_object* object = nullptr;
    vcjson_value* val = nullptr;
    vcjson_number* number = nullptr;
    vcjson_object_iterator* iter = nullptr;
    const char* ikey = nullptr;
    char key[20];

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create an object instance. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_create(&object, alloc));

    /* create and put key-value pairs. */
    for (int i = 0; i < 10; ++i)
    {
        /* create the key. */
        snprintf(key, sizeof(key), "%d", i);

        /* create a number. */
        TEST_ASSERT(STATUS_SUCCESS == vcjson_number_create(&number, alloc, i));

        /* wrap the number in a value. */
        TEST_ASSERT(
            STATUS_SUCCESS
                == vcjson_value_create_from_number(&val, alloc, number));

        /* put this value in the object. */
        TEST_ASSERT(STATUS_SUCCESS == vcjson_object_put(object, key, val));
    }

    /* create the iterator. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_object_iterator_create(&iter, object));

    /* verify that we can iterate over all elements. */
    for (int i = 0; i < 10; ++i)
    {
        /* create the key. */
        snprintf(key, sizeof(key), "%d", i);

        /* key the key-value pair from the iterator. */
        TEST_ASSERT(
            STATUS_SUCCESS == vcjson_object_iterator_value(&ikey, &val, iter));

        /* the keys should match. */
        TEST_EXPECT(0 == strcmp(key, ikey));

        /* the value type should be a number. */
        TEST_ASSERT(VCJSON_VALUE_TYPE_NUMBER == vcjson_value_type(val));

        /* we can get the number value. */
        TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_number(&number, val));

        /* the number value should match. */
        TEST_EXPECT((double)i == vcjson_number_value(number));

        /* get the next value. */
        if (i < 9)
        {
            TEST_ASSERT(STATUS_SUCCESS == vcjson_object_iterator_next(iter));
        }
        else
        {
            TEST_ASSERT(
                ERROR_VCJSON_ITERATOR_END == vcjson_object_iterator_next(iter));
        }
    }

    /* getting a value from the spent iterator should return an error. */
    TEST_EXPECT(
        ERROR_VCJSON_ITERATOR_BAD
            == vcjson_object_iterator_value(&ikey, &val, iter));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_object_iterator_resource_handle(iter)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_object_resource_handle(object)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(allocator_resource_handle(alloc)));
}
