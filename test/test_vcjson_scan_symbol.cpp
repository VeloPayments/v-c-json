/**
 * \file test/test_vcjson_scan_symbol.cpp
 *
 * \brief Unit tests for vcjson_scan_symbol.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <cstring>
#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

#include "../src/vcjson_internal.h"

/**
 * We can scan an EOF.
 */
TEST(scan_eof)
{
    const char* INPUT = R"()";
    size_t size = strlen(INPUT);
    int symbol = -10;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be LEFT_BRACKET. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_SPECIAL_EOF == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(0 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(0 == offset);
}

/**
 * We can scan a left bracket.
 */
TEST(scan_left_bracket)
{
    const char* INPUT = R"([)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be LEFT_BRACKET. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LEFT_BRACKET == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(0 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * We can scan a right bracket.
 */
TEST(scan_right_bracket)
{
    const char* INPUT = R"(])";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be RIGHT_BRACKET. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_RIGHT_BRACKET == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(0 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * We can scan a left brace.
 */
TEST(scan_left_brace)
{
    const char* INPUT = R"({)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be LEFT_BRACE. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LEFT_BRACE == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(0 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * We can scan a right brace.
 */
TEST(scan_right_brace)
{
    const char* INPUT = R"(})";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be RIGHT_BRACE. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_RIGHT_BRACE == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(0 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * We can scan a colon.
 */
TEST(scan_colon)
{
    const char* INPUT = R"(:)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be COLON. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_COLON == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(0 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * We can scan a comma.
 */
TEST(scan_comma)
{
    const char* INPUT = R"(,)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be COMMA. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_COMMA == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(0 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * Whitespace before a symbol is consumed.
 */
TEST(ignore_whitespace)
{
    const char* INPUT = " \t\n\r   ,";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be COMMA. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_COMMA == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(7 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(7 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(8 == offset);
}

/**
 * Whitespace before EOF is consumed.
 */
TEST(ignore_whitespace_EOF)
{
    const char* INPUT = " \t\n\r   ";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be EOF. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_SPECIAL_EOF == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(7 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(7 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(7 == offset);
}

/**
 * An empty string can be scanned.
 */
TEST(simple_string)
{
    const char* INPUT = R"("")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be STRING. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_STRING == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * An unterminated empty string is an error.
 */
TEST(unterminated_simple_string)
{
    const char* INPUT = R"(")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_53098ec5_2c31_46f2_8b6c_17c0674708b5
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to end-of-input. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * A string full of whitespace is valid.
 */
TEST(whitespace_string)
{
    const char* INPUT = "\"  \t\r\n   \"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* the symbol should be STRING. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_STRING == symbol);
    /* startpos should be correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos should be correct. */
    TEST_EXPECT(9 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(10 == offset);
}

/**
 * A continuation byte with no start byte is invalid.
 */
TEST(string_with_unpaired_continuation_byte)
{
    const char* INPUT = "\"\x82\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_f44c9426_32bc_4191_9e80_a508171a6d41
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * A string with a UTF-8 2 byte start and no continuation byte is invalid.
 */
TEST(string_with_truncated_2byte_sequence)
{
    const char* INPUT = "\"\xc2\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_40221afc_d49e_404e_8b18_1be7e48a2ecf
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(2 == startpos);
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * An "overlong" two byte sequence is invalid.
 */
TEST(string_with_overlong_2byte_sequence)
{
    const char* INPUT = "\"\xc0\x80\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_8a7564a9_b682_4ea0_88bc_629395331c5f
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A string with a valid two-byte UTF-8 sequence can be parsed.
 */
TEST(string_with_valid_2byte_sequence)
{
    const char* INPUT = R"("très bien")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(11 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(12 == offset);
}

/**
 * A string with a UTF-8 3 byte start and no continuation byte is invalid.
 */
TEST(string_with_truncated_3byte_sequence_1)
{
    const char* INPUT = "\"\xe2\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_3371878e_c9c8_44b8_be57_60d9ace6bd4f
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(2 == startpos);
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A string with a UTF-8 3 byte start and only one continuation byte is invalid.
 */
TEST(string_with_truncated_3byte_sequence_2)
{
    const char* INPUT = "\"\xe2\x82\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_3371878e_c9c8_44b8_be57_60d9ace6bd4f
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(3 == startpos);
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A string with a valid three-byte UTF-8 sequence can be parsed.
 */
TEST(string_with_valid_3byte_sequence)
{
    const char* INPUT = R"("30€")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(6 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(7 == offset);
}

/**
 * An "overlong" three byte sequence is invalid.
 */
TEST(string_with_overlong_3byte_sequence)
{
    const char* INPUT = "\"\xe0\x80\x80\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_696f7da7_6349_4356_827b_01fba3599d4c
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A codepoint that falls within the UTF-16 surrogate pair range is invalid.
 */
TEST(string_with_surrogate_pair_codepoint)
{
    const char* INPUT = "\"\xed\xa0\x9a\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_d64622e3_3077_46e2_84a4_d21b9a6dc02f
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A string with a UTF-8 4 byte start and no continuation byte is invalid.
 */
TEST(string_with_truncated_4byte_sequence_1)
{
    const char* INPUT = "\"\xf1\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(2 == startpos);
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A string with a UTF-8 4 byte start and only one continuation byte is invalid.
 */
TEST(string_with_truncated_4byte_sequence_2)
{
    const char* INPUT = "\"\xf1\x82\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(3 == startpos);
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A string with a UTF-8 4 byte start and only two continuation bytes is
 * invalid.
 */
TEST(string_with_truncated_4byte_sequence_3)
{
    const char* INPUT = "\"\xf1\x82\x82\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(4 == startpos);
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}

/**
 * A string with a valid four-byte UTF-8 sequence can be parsed.
 */
TEST(string_with_valid_4byte_sequence)
{
    const char* INPUT = R"("𓍹𓄿𓃬𓎟𓊃𓇋𓈖𓂧𓂋𓊃𓍺")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(45 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(46 == offset);
}

/**
 * An "overlong" four byte sequence is invalid.
 */
TEST(string_with_overlong_4byte_sequence)
{
    const char* INPUT = "\"\xf0\x80\x80\x80\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_dbf3f552_c055_4616_96bf_c48f17893162
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}

/**
 * An "out of range" four byte sequence is invalid.
 */
TEST(string_with_out_of_range_4byte_sequence)
{
    const char* INPUT = "\"\xf4\x90\x80\x80\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_3673de25_4603_4492_a25a_31752e9395fc
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}
