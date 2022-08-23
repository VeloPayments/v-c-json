/**
 * \file test/test_vcjson_emit_string.cpp
 *
 * \brief Unit tests for vcjson_emit_string.
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

TEST_SUITE(vcjson_emit);

/**
 * Verify that we can emit a null value.
 */
TEST(vcjson_emit_null)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"(null)";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a true value.
 */
TEST(vcjson_emit_true)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"(true)";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a false value.
 */
TEST(vcjson_emit_false)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"(false)";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a number value.
 */
TEST(vcjson_emit_number)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"(-1.250000)";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit an empty string.
 */
TEST(vcjson_emit_empty_string)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a simple string.
 */
TEST(vcjson_emit_simple_string)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("This is a test.")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a string with a quoted escape.
 */
TEST(vcjson_emit_quoted_escape)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("\"")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a string with a quoted solidus.
 */
TEST(vcjson_emit_quoted_solidus)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("\/")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a string with a quoted reverse solidus.
 */
TEST(vcjson_emit_quoted_reverse_solidus)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("\\")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a string with a quoted backspace.
 */
TEST(vcjson_emit_quoted_backspace)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("\b")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a string with a quoted formfeed.
 */
TEST(vcjson_emit_quoted_formfeed)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("\f")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a string with a quoted linefeed.
 */
TEST(vcjson_emit_quoted_linefeed)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("\n")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a string with a quoted carriage return.
 */
TEST(vcjson_emit_quoted_carriage_return)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("\r")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit a string with a quoted tab.
 */
TEST(vcjson_emit_quoted_tab)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"("\t")";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit an empty object.
 */
TEST(vcjson_emit_empty_object)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"({})";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit an object with one value.
 */
TEST(vcjson_emit_object_with_single_value)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"({"foo":true})";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit an object with multiple values.
 */
TEST(vcjson_emit_object_with_multiple_values)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"({"1":"foo","2":"bar","3":"baz"})";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit an empty array.
 */
TEST(vcjson_emit_empty_array)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"([])";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit an array with a single element.
 */
TEST(vcjson_emit_array_with_single_element)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"([null])";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Verify that we can emit an array with multiple elements.
 */
TEST(vcjson_emit_array_with_multiple_elements)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    vcjson_string* out = nullptr;
    size_t error_begin = 0xffff;
    size_t error_end = 0xffff;
    const char* INPUT = R"([null,null,null])";
    const char* outstr;
    size_t outstrlen;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* we can parse this string. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, INPUT));

    /* we can emit this value. */
    TEST_ASSERT(STATUS_SUCCESS == vcjson_emit_string(&out, alloc, value));

    /* get the string value. */
    outstr = vcjson_string_value(out, &outstrlen);

    /* the value should be set. */
    TEST_ASSERT(nullptr != out);
    /* the length should be correct. */
    TEST_ASSERT(outstrlen == strlen(INPUT));
    /* the strings should match. */
    TEST_EXPECT(0 == memcmp(INPUT, outstr, outstrlen));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcjson_string_resource_handle(out)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(vcjson_value_resource_handle(value)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}
