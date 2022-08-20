/**
 * \file test/test_vcjson_parse.cpp
 *
 * \brief Unit tests for vcjson_parse.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <cstring>
#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

using namespace std;

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_parse);

/**
 * Verify that an empty string causes an error.
 */
TEST(vcjson_parse_empty_string)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing an empty string causes a parse error. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_b369f991_4e11_4210_9076_ddc799d5bf44
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, ""));

    /* when an error is encountered, the error positions are set, and value is
     * NULL. */
    TEST_EXPECT(0 == error_begin);
    TEST_EXPECT(0 == error_end);
    TEST_EXPECT(nullptr == value);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse JSON true.
 */
TEST(vcjson_parse_true)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_bool* boolvalue = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "true"));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is boolean. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(value));
    /* verify that this value is true. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_bool(&boolvalue, value));
    TEST_EXPECT(VCJSON_TRUE == boolvalue);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse JSON true with whitespace surrounding it.
 */
TEST(vcjson_parse_true_whitespace)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_bool* boolvalue = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "  true\n\t "));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is boolean. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(value));
    /* verify that this value is true. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_bool(&boolvalue, value));
    TEST_EXPECT(VCJSON_TRUE == boolvalue);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that parsing true with any other symbol after it causes an error.
 */
TEST(vcjson_parse_true_true)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing multiple symbols fails. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_b87aa047_46c8_453c_aa3c_bb7c1dd70402
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "true true"));

    /* the error begin / end should point to the second symbol. */
    TEST_EXPECT(5 == error_begin);
    TEST_EXPECT(8 == error_end);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse JSON false.
 */
TEST(vcjson_parse_false)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_bool* boolvalue = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "false"));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is boolean. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(value));
    /* verify that this value is true. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_bool(&boolvalue, value));
    TEST_EXPECT(VCJSON_FALSE == boolvalue);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse JSON false with whitespace surrounding it.
 */
TEST(vcjson_parse_false_whitespace)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_bool* boolvalue = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "  false\n\t "));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is boolean. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(value));
    /* verify that this value is true. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_bool(&boolvalue, value));
    TEST_EXPECT(VCJSON_FALSE == boolvalue);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that parsing false with any other symbol after it causes an error.
 */
TEST(vcjson_parse_false_true)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing multiple symbols fails. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_b87aa047_46c8_453c_aa3c_bb7c1dd70402
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "false true"));

    /* the error begin / end should point to the second symbol. */
    TEST_EXPECT(6 == error_begin);
    TEST_EXPECT(9 == error_end);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse JSON null.
 */
TEST(vcjson_parse_null)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "null"));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is null. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_NULL == vcjson_value_type(value));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse JSON null with whitespace surrounding it.
 */
TEST(vcjson_parse_null_whitespace)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "  null\n\t "));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is null. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_NULL == vcjson_value_type(value));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that parsing null with any other symbol after it causes an error.
 */
TEST(vcjson_parse_null_true)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing multiple symbols fails. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_b87aa047_46c8_453c_aa3c_bb7c1dd70402
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "null true"));

    /* the error begin / end should point to the second symbol. */
    TEST_EXPECT(5 == error_begin);
    TEST_EXPECT(8 == error_end);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse an empty string.
 */
TEST(vcjson_parse_empty_string_value)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* stringvalue = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, R"("")"));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is string. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_STRING == vcjson_value_type(value));
    /* get the string value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_string(&stringvalue, value));
    /* the string value should be empty. */
    size_t length;
    const char* str = vcjson_string_value(stringvalue, &length);
    TEST_ASSERT(length == 1);
    TEST_EXPECT(!memcmp("", str, length));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse a simple string.
 */
TEST(vcjson_parse_simple_string_value)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* stringvalue = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"("test string")";
    const char* EXPECTED_STRING = "test string";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is string. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_STRING == vcjson_value_type(value));
    /* get the string value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_string(&stringvalue, value));
    /* the string value should be empty. */
    size_t length;
    const char* str = vcjson_string_value(stringvalue, &length);
    TEST_ASSERT(length - 1 == strlen(EXPECTED_STRING));
    TEST_EXPECT(!memcmp(EXPECTED_STRING, str, length));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse a string with escape values.
 */
TEST(vcjson_parse_simple_escape_string_value)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* stringvalue = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"("tab\t newline\n cr\r quote\"\\\/\b\f")";
    const char* EXPECTED_STRING = "tab\t newline\n cr\r quote\"\\/\b\f";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is string. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_STRING == vcjson_value_type(value));
    /* get the string value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_string(&stringvalue, value));
    /* the string value should be empty. */
    size_t length;
    const char* str = vcjson_string_value(stringvalue, &length);
    TEST_ASSERT(length - 1 == strlen(EXPECTED_STRING));
    TEST_EXPECT(!memcmp(EXPECTED_STRING, str, length));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}
