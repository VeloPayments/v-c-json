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
    const char* INPUT = "\"     \"";
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
    TEST_EXPECT(6 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(7 == offset);
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

    /* the symbol should be STRING. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_STRING == symbol);
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

    /* the symbol should be STRING. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_STRING == symbol);
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

    /* the symbol should be STRING. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_STRING == symbol);
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

/**
 * A string with an unescaped whitespace control sequence is invalid.
 */
TEST(string_with_unescaped_ws_control)
{
    const char* INPUT = "\"\t\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_903c7867_9325_4576_85ac_3e312735def9
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * A string with an unescaped control sequence is invalid.
 */
TEST(string_with_unescaped_control)
{
    const char* INPUT = "\"\f\"";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_e08745b4_8269_4c1d_bebe_474170354990
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(1 == startpos);
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * A string with a partial escape sequence is invalid.
 */
TEST(string_with_partial_escape)
{
    const char* INPUT = R"("\)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_89b93262_852c_4ab7_a41c_2da08a73a850
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos and endpos should point to the error. */
    TEST_EXPECT(2 == startpos);
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * A quote can be escaped.
 */
TEST(string_with_quote_escape)
{
    const char* INPUT = R"("\"")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A backslash (reverse solidus) can be escaped.
 */
TEST(string_with_reverse_solidus_escape)
{
    const char* INPUT = R"("\\")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A forward slash (solidus) can be escaped.
 */
TEST(string_with_solidus_escape)
{
    const char* INPUT = R"("\/")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A backspace can be escaped.
 */
TEST(string_with_backspace_escape)
{
    const char* INPUT = R"("\b")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A form feed can be escaped.
 */
TEST(string_with_form_feed_escape)
{
    const char* INPUT = R"("\f")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A line feed can be escaped.
 */
TEST(string_with_line_feed_escape)
{
    const char* INPUT = R"("\n")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A carriage return can be escaped.
 */
TEST(string_with_carriage_return_escape)
{
    const char* INPUT = R"("\r")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A horizontal tab can be escaped.
 */
TEST(string_with_horizontal_tab_escape)
{
    const char* INPUT = R"("\t")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A JSON unicode escape sequence can be escaped.
 */
TEST(string_with_JSON_unicode_escape)
{
    const char* INPUT = R"("\u2b4b")";
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
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(7 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(8 == offset);
}

/**
 * A partial JSON unicode escape sequence is invalid.
 */
TEST(string_with_partial_JSON_unicode_escape_1)
{
    const char* INPUT = R"("\u")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(3 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A partial JSON unicode escape sequence is invalid.
 */
TEST(string_with_partial_JSON_unicode_escape_2)
{
    const char* INPUT = R"("\u)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(3 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A partial JSON unicode escape sequence is invalid.
 */
TEST(string_with_partial_JSON_unicode_escape_3)
{
    const char* INPUT = R"("\u1")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(4 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}

/**
 * A partial JSON unicode escape sequence is invalid.
 */
TEST(string_with_partial_JSON_unicode_escape_4)
{
    const char* INPUT = R"("\u1)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(4 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A partial JSON unicode escape sequence is invalid.
 */
TEST(string_with_partial_JSON_unicode_escape_5)
{
    const char* INPUT = R"("\u1a")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(5 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(5 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(6 == offset);
}

/**
 * A partial JSON unicode escape sequence is invalid.
 */
TEST(string_with_partial_JSON_unicode_escape_6)
{
    const char* INPUT = R"("\u1a)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(5 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(5 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}

/**
 * A partial JSON unicode escape sequence is invalid.
 */
TEST(string_with_partial_JSON_unicode_escape_7)
{
    const char* INPUT = R"("\u1a7")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(6 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(6 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(7 == offset);
}

/**
 * A partial JSON unicode escape sequence is invalid.
 */
TEST(string_with_partial_JSON_unicode_escape_8)
{
    const char* INPUT = R"("\u1a7)";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(6 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(6 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(6 == offset);
}

/**
 * Any other escape sequence is invalid.
 */
TEST(string_with_invalid_escape)
{
    const char* INPUT = R"("\q")";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_65b96e7e_25c7_4f2a_9c8d_bce126776faa
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(2 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A 't' is an unknown symbol.
 */
TEST(unknown_t)
{
    const char* INPUT = "t";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_c48ee2cb_30dc_4264_90f8_c8545f131429
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(1 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * A 'tx' is an unknown symbol.
 */
TEST(unknown_tx)
{
    const char* INPUT = "tx";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_c48ee2cb_30dc_4264_90f8_c8545f131429
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(1 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * A 'tr' is an unknown symbol.
 */
TEST(unknown_tr)
{
    const char* INPUT = "tr";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_b71d4bff_342e_45ce_b1d5_e77f472ebc95
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(2 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * A 'trx' is an unknown symbol.
 */
TEST(unknown_trx)
{
    const char* INPUT = "trx";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_b71d4bff_342e_45ce_b1d5_e77f472ebc95
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(2 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A 'tru' is an unknown symbol.
 */
TEST(unknown_tru)
{
    const char* INPUT = "tru";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_ddb74f50_c250_463f_ad04_7fb6a181790d
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(3 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A 'trux' is an unknown symbol.
 */
TEST(unknown_trux)
{
    const char* INPUT = "trux";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_ddb74f50_c250_463f_ad04_7fb6a181790d
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(3 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A 'true' can be scanned. 
 */
TEST(true_happy)
{
    const char* INPUT = "true";
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

    /* symbol is TRUE. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_TRUE == symbol);
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A 'true' trailing whitespace can be scanned.
 */
TEST(true_with_whitespace)
{
    const char* INPUT = "true ";
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

    /* symbol is TRUE. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_TRUE == symbol);
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A 'truep' is an error.
 */
TEST(unknown_truep)
{
    const char* INPUT = "truep";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_299d80db_2eec_4ed3_9717_1b3ecd188c4c
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(4 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * An 'f' is an unknown symbol.
 */
TEST(unknown_f)
{
    const char* INPUT = "f";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_9b5a8590_05d8_4c88_a45a_48b4ebf1ce3c
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(1 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(1 == offset);
}

/**
 * An 'fx' is an unknown symbol.
 */
TEST(unknown_fx)
{
    const char* INPUT = "fx";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_9b5a8590_05d8_4c88_a45a_48b4ebf1ce3c
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(1 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(1 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * A 'fa' is an unknown symbol.
 */
TEST(unknown_fa)
{
    const char* INPUT = "fa";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_19478855_7462_4d89_a9c9_ff500d4328c2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(2 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(2 == offset);
}

/**
 * A 'fax' is an unknown symbol.
 */
TEST(unknown_fax)
{
    const char* INPUT = "fax";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_19478855_7462_4d89_a9c9_ff500d4328c2
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(2 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(2 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A 'fal' is an unknown symbol.
 */
TEST(unknown_fal)
{
    const char* INPUT = "fal";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_2783f9f0_77a0_4b4c_b561_8dc932f89ea3
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(3 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(3 == offset);
}

/**
 * A 'falx' is an unknown symbol.
 */
TEST(unknown_falx)
{
    const char* INPUT = "falx";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_2783f9f0_77a0_4b4c_b561_8dc932f89ea3
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(3 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(3 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A 'fals' is an unknown symbol.
 */
TEST(unknown_fals)
{
    const char* INPUT = "fals";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_83ad3fa4_4479_4fc0_9f28_912c2c619473
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(4 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(4 == offset);
}

/**
 * A 'falsx' is an unknown symbol.
 */
TEST(unknown_falsx)
{
    const char* INPUT = "falsx";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should fail. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_83ad3fa4_4479_4fc0_9f28_912c2c619473
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(4 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}

/**
 * A 'false' can be scanned. 
 */
TEST(false_happy)
{
    const char* INPUT = "false";
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

    /* symbol is FALSE. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_FALSE == symbol);
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}

/**
 * A 'false' with trailing whitespace can be scanned. 
 */
TEST(false_with_whitespace)
{
    const char* INPUT = "false ";
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

    /* symbol is FALSE. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_FALSE == symbol);
    /* startpos is correct. */
    TEST_EXPECT(0 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(4 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}

/**
 * A 'falsep' is an error.
 */
TEST(unknown_falsep)
{
    const char* INPUT = "falsep";
    size_t size = strlen(INPUT);
    int symbol = -1;
    size_t startpos = 100;
    size_t endpos = 100;
    size_t offset = 0;

    /* scanning for a symbol should succeed. */
    TEST_ASSERT(
        ERROR_VCJSON_SCAN_299d80db_2eec_4ed3_9717_1b3ecd188c4c
            == vcjson_scan_symbol(
                    &symbol, &startpos, &endpos, INPUT, size, &offset));

    /* startpos is correct. */
    TEST_EXPECT(5 == startpos);
    /* endpos is correct. */
    TEST_EXPECT(5 == endpos);
    /* offset should be correct. */
    TEST_EXPECT(5 == offset);
}
