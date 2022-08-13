/**
 * \file test/test_vcjson_value.cpp
 *
 * \brief Unit tests for vcjson_value related functions.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>
#include <cstring>

using namespace std;

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

/**
 * \brief Test for number related values.
 */
TEST(value_number_basics)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_number* numberval = nullptr;
    const double EXPECTED_VALUE = 99.25;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a number. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_number_create(&numberval, alloc, EXPECTED_VALUE));

    /* create a value from this number. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_value_create_from_number(&value, alloc, numberval));

    /* reset number value since it is now owned by the value. */
    numberval = nullptr;

    /* the type of this value is VCJSON_VALUE_TYPE_NUMBER. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_NUMBER == vcjson_value_type(value));

    /* get the number value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_number(&numberval, value));

    /* this number value is our expected value. */
    TEST_EXPECT(EXPECTED_VALUE == vcjson_number_value(numberval));

    /* clean up. */
    resource_release(vcjson_value_resource_handle(value));
    resource_release(allocator_resource_handle(alloc));
}

/**
 * \brief Test for string related values.
 */
TEST(value_string_basics)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* stringval = nullptr;
    const char* EXPECTED_VALUE = "This is a test.";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create a string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_string_create(&stringval, alloc, EXPECTED_VALUE));

    /* create a value from this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_value_create_from_string(&value, alloc, stringval));

    /* reset stringval value since it is now owned by the value. */
    stringval = nullptr;

    /* the type of this value is VCJSON_VALUE_TYPE_STRING. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_STRING == vcjson_value_type(value));

    /* get the string value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_string(&stringval, value));

    /* this string value is our expected value. */
    TEST_EXPECT(0 == strcmp(EXPECTED_VALUE, vcjson_string_value(stringval)));

    /* clean up. */
    resource_release(vcjson_value_resource_handle(value));
    resource_release(allocator_resource_handle(alloc));
}

/**
 * \brief Test for object related values.
 */
TEST(value_object_basics)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_object* objectval = nullptr;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create an object. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_object_create(&objectval, alloc));

    /* create a value from this object. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_value_create_from_object(&value, alloc, objectval));

    /* reset object value since it is now owned by the value. */
    objectval = nullptr;

    /* the type of this value is VCJSON_VALUE_TYPE_OBJECT. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_OBJECT == vcjson_value_type(value));

    /* get the object value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_object(&objectval, value));

    /* clean up. */
    resource_release(vcjson_value_resource_handle(value));
    resource_release(allocator_resource_handle(alloc));
}

/**
 * \brief Test for array related values.
 */
TEST(value_array_basics)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_array* arrayval = nullptr;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create an array. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_array_create(&arrayval, alloc, 3));

    /* create a value from this array. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_value_create_from_array(&value, alloc, arrayval));

    /* reset array value since it is now owned by the value. */
    arrayval = nullptr;

    /* the type of this value is VCJSON_VALUE_TYPE_ARRAY. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_ARRAY == vcjson_value_type(value));

    /* get the array value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_array(&arrayval, value));

    /* clean up. */
    resource_release(vcjson_value_resource_handle(value));
    resource_release(allocator_resource_handle(alloc));
}
