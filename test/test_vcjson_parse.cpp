/**
 * \file test/test_vcjson_parse.cpp
 *
 * \brief Unit tests for vcjson_parse.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_parse);

#if 0
/**
 * Verify that an empty string causes an error.
 */
TEST(vcjson_parse_empty_string)
{
    allocator* alloc = nullptr;
    vcjson_value* value = nullptr;
    size_t error_begin = 0;
    size_t error_end = 0;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* parsing an empty string causes a parse error. */
    TEST_EXPECT(
        ERROR_VCJSON_PARSE_b369f991_4e11_4210_9076_ddc799d5bf44
            == vcjson_parse_string(
                    &value, &error_begin, &error_end, alloc, ""));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS
        == resource_release(allocator_resource_handle(alloc)));
}
#endif
