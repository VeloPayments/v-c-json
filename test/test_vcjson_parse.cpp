/**
 * \file test/test_vcjson_parse.cpp
 *
 * \brief Unit tests for vcjson_parse.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <cmath>
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
 * Verify that we can parse a number.
 */
TEST(vcjson_parse_number)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_number* numberval = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const double epsilon = 0.0001;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing multiple symbols fails. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, "3.14"));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is number. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_NUMBER == vcjson_value_type(value));
    /* get the number value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_number(&numberval, value));
    /* verify that the number value matches what we expect. */
    TEST_EXPECT(fabs(vcjson_number_value(numberval) - 3.14) < epsilon);

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
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
    TEST_ASSERT(length == 0);
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
    TEST_ASSERT(length == strlen(EXPECTED_STRING));
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
    TEST_ASSERT(length == strlen(EXPECTED_STRING));
    TEST_EXPECT(!memcmp(EXPECTED_STRING, str, length));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/* TODO - add unit test for basic \u escape values. */
/* TODO - add unit test for basic \u escape surrogate pairs. */

/**
 * Verify that we can parse an empty object.
 */
TEST(vcjson_parse_empty_object)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({})";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is object. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_OBJECT == vcjson_value_type(value));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse an object with a single member.
 */
TEST(vcjson_parse_object_with_single_member)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_object* obj = nullptr;
    vcjson_object_iterator* iter = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({ "foo" : "bar" })";
    const vcjson_string* key = nullptr;
    vcjson_value* val = nullptr;
    const char* keystrval = nullptr;
    size_t keystrlength = 0U;
    vcjson_string* valstr = nullptr;
    const char* valstrstr = nullptr;
    size_t valstrlength = 0U;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is object. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_OBJECT == vcjson_value_type(value));
    /* get the object. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_object(&obj, value));
    /* the object should have one member. */
    TEST_EXPECT(1 == vcjson_object_elements(obj));
    /* create an iterator over these members. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_object_iterator_create(&iter, obj));
    /* get the value of the first member. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_iterator_value(&key, &val, iter));
    /* the key's string value should be equal to "foo". */
    keystrval = vcjson_string_value(key, &keystrlength);
    TEST_ASSERT(keystrlength == strlen("foo"));
    TEST_EXPECT(!memcmp("foo", keystrval, keystrlength));
    /* the value should be of type string. */
    TEST_ASSERT(VCJSON_VALUE_TYPE_STRING == vcjson_value_type(val));
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_string(&valstr, val));
    /* the value's string value should be equal to "bar". */
    valstrstr = vcjson_string_value(valstr, &valstrlength);
    TEST_ASSERT(valstrlength == strlen("bar"));
    TEST_EXPECT(!memcmp("bar", valstrstr, valstrlength));

    /* there are no more elements. */
    TEST_ASSERT(ERROR_VCJSON_ITERATOR_END == vcjson_object_iterator_next(iter));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse an object with multiple members.
 */
TEST(vcjson_parse_object_with_multiple_members)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_object* obj = nullptr;
    vcjson_object_iterator* iter = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({ "1" : "foo", "2" : "bar", "3" : "baz" })";
    const vcjson_string* key = nullptr;
    vcjson_value* val = nullptr;
    const char* keystrval = nullptr;
    size_t keystrlength = 0U;
    vcjson_string* valstr = nullptr;
    const char* valstrstr = nullptr;
    size_t valstrlength = 0U;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is object. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_OBJECT == vcjson_value_type(value));
    /* get the object. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_object(&obj, value));
    /* the object should have three members. */
    TEST_EXPECT(3 == vcjson_object_elements(obj));
    /* create an iterator over these members. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_object_iterator_create(&iter, obj));

    /* get the value of the first member. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_iterator_value(&key, &val, iter));
    /* the key's string value should be equal to "1". */
    keystrval = vcjson_string_value(key, &keystrlength);
    TEST_ASSERT(keystrlength == strlen("1"));
    TEST_EXPECT(!memcmp("1", keystrval, keystrlength));
    /* the value should be of type string. */
    TEST_ASSERT(VCJSON_VALUE_TYPE_STRING == vcjson_value_type(val));
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_string(&valstr, val));
    /* the value's string value should be equal to "foo". */
    valstrstr = vcjson_string_value(valstr, &valstrlength);
    TEST_ASSERT(valstrlength == strlen("foo"));
    TEST_EXPECT(!memcmp("foo", valstrstr, valstrlength));

    /* we can get the next element. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_object_iterator_next(iter));

    /* get the value of the second member. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_iterator_value(&key, &val, iter));
    /* the key's string value should be equal to "2". */
    keystrval = vcjson_string_value(key, &keystrlength);
    TEST_ASSERT(keystrlength == strlen("2"));
    TEST_EXPECT(!memcmp("2", keystrval, keystrlength));
    /* the value should be of type string. */
    TEST_ASSERT(VCJSON_VALUE_TYPE_STRING == vcjson_value_type(val));
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_string(&valstr, val));
    /* the value's string value should be equal to "bar". */
    valstrstr = vcjson_string_value(valstr, &valstrlength);
    TEST_ASSERT(valstrlength == strlen("bar"));
    TEST_EXPECT(!memcmp("bar", valstrstr, valstrlength));

    /* we can get the next element. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_object_iterator_next(iter));

    /* get the value of the third member. */
    TEST_ASSERT(
        STATUS_SUCCESS == vcjson_object_iterator_value(&key, &val, iter));
    /* the key's string value should be equal to "3". */
    keystrval = vcjson_string_value(key, &keystrlength);
    TEST_ASSERT(keystrlength == strlen("3"));
    TEST_EXPECT(!memcmp("3", keystrval, keystrlength));
    /* the value should be of type string. */
    TEST_ASSERT(VCJSON_VALUE_TYPE_STRING == vcjson_value_type(val));
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_string(&valstr, val));
    /* the value's string value should be equal to "baz". */
    valstrstr = vcjson_string_value(valstr, &valstrlength);
    TEST_ASSERT(valstrlength == strlen("baz"));
    TEST_EXPECT(!memcmp("baz", valstrstr, valstrlength));

    /* there are no more elements. */
    TEST_ASSERT(ERROR_VCJSON_ITERATOR_END == vcjson_object_iterator_next(iter));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An empty object missing a closing brace will fail.
 */
TEST(bad_object_1)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({)";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_ffa4f503_8429_49f4_bbf2_8a91276d234c
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An object with a single member and a missing closing brace will fail.
 */
TEST(bad_object_2)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({"foo":"bar")";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_ffa4f503_8429_49f4_bbf2_8a91276d234c
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An object with multiple members and a missing closing brace will fail.
 */
TEST(bad_object_3)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({"1":"foo","2":"bar")";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_ffa4f503_8429_49f4_bbf2_8a91276d234c
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An object with multiple members ending in comma and no closing brace will
 * fail.
 */
TEST(bad_object_4)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({"1":"foo","2":"bar",)";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_ffa4f503_8429_49f4_bbf2_8a91276d234c
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * A member list starting with a comma fails.
 */
TEST(bad_object_5)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({,})";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_b664370d_72ce_4778_8f68_30c7dc3b14e5
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An object with multiple members not separated by commas will fail.
 */
TEST(bad_object_6)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({"1":"foo" "2":"bar"})";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_1e9e755f_b416_4f9a_95e7_5acd39a09b47
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An object with a member key and no value will fail.
 */
TEST(bad_object_7)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({"1"})";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_be519e92_b2a0_44a4_84f1_3d506fd3f54d
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An object with a member key, a colon, and no value will fail.
 */
TEST(bad_object_8)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({"1":})";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_fb48555e_2ed9_414a_841e_0d5b39b52090
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An object with a single member and a comma is invalid.
 */
TEST(bad_object_9)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"({"1":"foo",})";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_69c86e4f_d981_402d_a4fd_c051b97e821a
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse an empty array.
 */
TEST(vcjson_parse_empty_array)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_array* array = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([])";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is array. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_ARRAY == vcjson_value_type(value));
    /* get the array. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_array(&array, value));
    /* this array is empty. */
    TEST_ASSERT(0 == vcjson_array_size(array));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse an array with one element.
 */
TEST(vcjson_parse_array_with_one_element)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_array* array = nullptr;
    vcjson_value* elemval = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([true])";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is array. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_ARRAY == vcjson_value_type(value));
    /* get the array. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_array(&array, value));
    /* this array has 1 element. */
    TEST_ASSERT(1 == vcjson_array_size(array));
    /* get the 0th element. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_array_get(&elemval, array, 0));
    /* the type of this value is boolean. */
    TEST_ASSERT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(elemval));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can parse an array with multiple elements.
 */
TEST(vcjson_parse_array_with_multiple_elements)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_array* array = nullptr;
    vcjson_value* elemval = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([true,false,null])";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* verify that the value is set. */
    TEST_ASSERT(nullptr != value);
    /* verify that the value type is array. */
    TEST_EXPECT(VCJSON_VALUE_TYPE_ARRAY == vcjson_value_type(value));
    /* get the array. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_value_get_array(&array, value));
    /* this array has 3 element. */
    TEST_ASSERT(3 == vcjson_array_size(array));

    /* get the 0th element. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_array_get(&elemval, array, 0));
    /* the type of this value is boolean. */
    TEST_ASSERT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(elemval));

    /* get the 1st element. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_array_get(&elemval, array, 1));
    /* the type of this value is boolean. */
    TEST_ASSERT(VCJSON_VALUE_TYPE_BOOL == vcjson_value_type(elemval));

    /* get the 2nd element. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_array_get(&elemval, array, 2));
    /* the type of this value is null. */
    TEST_ASSERT(VCJSON_VALUE_TYPE_NULL == vcjson_value_type(elemval));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An empty array with no closing bracket is invalid.
 */
TEST(bad_array_1)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([)";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_c207ee84_a90b_4d01_9314_a769a460819a
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An array with an element and no closing bracket is invalid.
 */
TEST(bad_array_2)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([null)";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_da3c5b50_0456_4acd_904b_2a72464e59ae
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An array with an element, a comma, and no closing bracket is invalid.
 */
TEST(bad_array_3)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([null,)";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_c207ee84_a90b_4d01_9314_a769a460819a
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An array with a comma and no closing bracket is invalid.
 */
TEST(bad_array_4)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([,)";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_4b143e34_8ab5_4a34_b79c_905f66b62511
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An array with a comma and a bracket is invalid.
 */
TEST(bad_array_5)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([,])";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_4b143e34_8ab5_4a34_b79c_905f66b62511
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}

/**
 * An array with an element, a comma, and a closing bracket is invalid.
 */
TEST(bad_array_6)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT_STRING = R"([null,])";

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing succeeds. */
    TEST_ASSERT(
        ERROR_VCJSON_PARSE_e02e6452_eedc_4049_aad0_f79cbf7442a2
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT_STRING));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}
