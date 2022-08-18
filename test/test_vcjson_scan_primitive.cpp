/**
 * \file test/test_vcjson_scan_primitive.cpp
 *
 * \brief Unit tests for vcjson_scan_primitive.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <cstring>
#include <minunit/minunit.h>
#include <vcjson/vcjson.h>

#include "../src/vcjson_internal.h"

using namespace std;

RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(vcjson_scan_primitive);

/**
 * Verify that we can scan an EOF.
 */
TEST(scan_eof)
{
    const char* INPUT = "";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be EOF. */
    TEST_EXPECT(VCJSON_LEXER_SYMBOL_SPECIAL_EOF == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the offset should be 0. */
    TEST_EXPECT(0 == offset);
}

/**
 * Verify that we can scan a colon.
 */
TEST(scan_colon)
{
    const char* INPUT = ":";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be COLON. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_COLON == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be ':' */
    TEST_EXPECT(':' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a comma.
 */
TEST(scan_comma)
{
    const char* INPUT = ",";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be COMMA. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_COMMA == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be ',' */
    TEST_EXPECT(',' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 0 digit.
 */
TEST(scan_digit_0)
{
    const char* INPUT = "0";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 0. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_0 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '0' */
    TEST_EXPECT('0' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 1 digit.
 */
TEST(scan_digit_1)
{
    const char* INPUT = "1";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 1. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_1 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '1' */
    TEST_EXPECT('1' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 2 digit.
 */
TEST(scan_digit_2)
{
    const char* INPUT = "2";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 2. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_2 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '2' */
    TEST_EXPECT('2' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 3 digit.
 */
TEST(scan_digit_3)
{
    const char* INPUT = "3";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 3. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_3 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '3' */
    TEST_EXPECT('3' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 4 digit.
 */
TEST(scan_digit_4)
{
    const char* INPUT = "4";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 4. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_4 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '4' */
    TEST_EXPECT('4' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 5 digit.
 */
TEST(scan_digit_5)
{
    const char* INPUT = "5";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 5. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_5 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '5' */
    TEST_EXPECT('5' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 6 digit.
 */
TEST(scan_digit_6)
{
    const char* INPUT = "6";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 6. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_6 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '6' */
    TEST_EXPECT('6' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 7 digit.
 */
TEST(scan_digit_7)
{
    const char* INPUT = "7";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 7. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_7 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '7' */
    TEST_EXPECT('7' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan an 8 digit.
 */
TEST(scan_digit_8)
{
    const char* INPUT = "8";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 8. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_8 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '8' */
    TEST_EXPECT('8' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a 9 digit.
 */
TEST(scan_digit_9)
{
    const char* INPUT = "9";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be 9. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DIGIT_9 == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '9' */
    TEST_EXPECT('9' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a dot.
 */
TEST(scan_dot)
{
    const char* INPUT = ".";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be DOT. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_DOT == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '.' */
    TEST_EXPECT('.' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a lower-case hex a.
 */
TEST(scan_hex_a)
{
    const char* INPUT = "a";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_a. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_a == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'a' */
    TEST_EXPECT('a' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan an upper-case hex A.
 */
TEST(scan_hex_A)
{
    const char* INPUT = "A";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_A. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_A == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'A' */
    TEST_EXPECT('A' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a lower-case hex b.
 */
TEST(scan_hex_b)
{
    const char* INPUT = "b";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_b. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_b == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'b' */
    TEST_EXPECT('b' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan an upper-case hex B.
 */
TEST(scan_hex_B)
{
    const char* INPUT = "B";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_B. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_B == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'B' */
    TEST_EXPECT('B' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a lower-case hex c.
 */
TEST(scan_hex_c)
{
    const char* INPUT = "c";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_c. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_c == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'c' */
    TEST_EXPECT('c' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan an upper-case hex C.
 */
TEST(scan_hex_C)
{
    const char* INPUT = "C";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_C. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_C == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'C' */
    TEST_EXPECT('C' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a lower-case hex d.
 */
TEST(scan_hex_d)
{
    const char* INPUT = "d";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_d. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_d == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'd' */
    TEST_EXPECT('d' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan an upper-case hex D.
 */
TEST(scan_hex_D)
{
    const char* INPUT = "D";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_D. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_D == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'D' */
    TEST_EXPECT('D' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a lower-case hex or exponent e.
 */
TEST(scan_hex_or_exponent_e)
{
    const char* INPUT = "e";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_OR_EXPONENT_e. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'e' */
    TEST_EXPECT('e' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan an upper-case hex or exponent E.
 */
TEST(scan_hex_or_exponent_E)
{
    const char* INPUT = "E";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_OR_EXPONENT_E. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_E == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'E' */
    TEST_EXPECT('E' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a lower-case hex f.
 */
TEST(scan_hex_f)
{
    const char* INPUT = "f";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_f. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_f == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'f' */
    TEST_EXPECT('f' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan an upper-case hex F.
 */
TEST(scan_hex_F)
{
    const char* INPUT = "F";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be HEX_F. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_HEX_F == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'F' */
    TEST_EXPECT('F' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a left brace.
 */
TEST(scan_left_brace)
{
    const char* INPUT = "{";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be LEFT_BRACE. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LEFT_BRACE == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '{' */
    TEST_EXPECT('{' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a left bracket.
 */
TEST(scan_left_bracket)
{
    const char* INPUT = "[";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be LEFT_BRACKET. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LEFT_BRACKET == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '[' */
    TEST_EXPECT('[' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a minus.
 */
TEST(scan_minus)
{
    const char* INPUT = "-";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be MINUS */
    TEST_EXPECT(VCJSON_LEXER_PRIM_MINUS == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '-' */
    TEST_EXPECT('-' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a plus.
 */
TEST(scan_plus)
{
    const char* INPUT = "+";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be PLUS */
    TEST_EXPECT(VCJSON_LEXER_PRIM_PLUS == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '+' */
    TEST_EXPECT('+' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a quote.
 */
TEST(scan_quote)
{
    const char* INPUT = "\"";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be QUOTE */
    TEST_EXPECT(VCJSON_LEXER_PRIM_QUOTE == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '"' */
    TEST_EXPECT('"' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a right brace.
 */
TEST(scan_right_brace)
{
    const char* INPUT = "}";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be RIGHT_BRACE */
    TEST_EXPECT(VCJSON_LEXER_PRIM_RIGHT_BRACE == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '}' */
    TEST_EXPECT('}' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that we can scan a right bracket.
 */
TEST(scan_right_bracket)
{
    const char* INPUT = "]";
    size_t size = strlen(INPUT);
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be RIGHT_BRACKET */
    TEST_EXPECT(VCJSON_LEXER_PRIM_RIGHT_BRACKET == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be ']' */
    TEST_EXPECT(']' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that NUL is a non-WS control character.
 */
TEST(scan_NUL)
{
    const char* INPUT = "\0";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\0' */
    TEST_EXPECT('\0' == INPUT[position]);
    TEST_EXPECT(0 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that SOH is a non-WS control character.
 */
TEST(scan_SOH)
{
    const char* INPUT = "\1";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\1' */
    TEST_EXPECT('\1' == INPUT[position]);
    TEST_EXPECT(1 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that STX is a non-WS control character.
 */
TEST(scan_STX)
{
    const char* INPUT = "\2";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\2' */
    TEST_EXPECT('\2' == INPUT[position]);
    TEST_EXPECT(2 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that ETX is a non-WS control character.
 */
TEST(scan_ETX)
{
    const char* INPUT = "\3";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\3' */
    TEST_EXPECT('\3' == INPUT[position]);
    TEST_EXPECT(3 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that EOT is a non-WS control character.
 */
TEST(scan_EOT)
{
    const char* INPUT = "\4";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\4' */
    TEST_EXPECT('\4' == INPUT[position]);
    TEST_EXPECT(4 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that ENQ is a non-WS control character.
 */
TEST(scan_ENQ)
{
    const char* INPUT = "\5";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\5' */
    TEST_EXPECT('\5' == INPUT[position]);
    TEST_EXPECT(5 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that ACK is a non-WS control character.
 */
TEST(scan_ACK)
{
    const char* INPUT = "\6";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\6' */
    TEST_EXPECT('\6' == INPUT[position]);
    TEST_EXPECT(6 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that BEL is a non-WS control character.
 */
TEST(scan_BEL)
{
    const char* INPUT = "\7";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\7' */
    TEST_EXPECT('\7' == INPUT[position]);
    TEST_EXPECT(7 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that BS is a non-WS control character.
 */
TEST(scan_BS)
{
    const char* INPUT = "\10";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\07' */
    TEST_EXPECT('\10' == INPUT[position]);
    TEST_EXPECT(8 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that TAB is a WS control character.
 */
TEST(scan_TAB)
{
    const char* INPUT = "\t";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\t' */
    TEST_EXPECT('\t' == INPUT[position]);
    TEST_EXPECT(9 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that LF is a WS control character.
 */
TEST(scan_LF)
{
    const char* INPUT = "\n";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\n' */
    TEST_EXPECT('\n' == INPUT[position]);
    TEST_EXPECT(10 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that VT is a non-WS control character.
 */
TEST(scan_VT)
{
    const char* INPUT = "\13";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\13' */
    TEST_EXPECT('\13' == INPUT[position]);
    TEST_EXPECT(11 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that FF is a non-WS control character.
 */
TEST(scan_FF)
{
    const char* INPUT = "\14";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\14' */
    TEST_EXPECT('\14' == INPUT[position]);
    TEST_EXPECT(12 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that CR is a WS control character.
 */
TEST(scan_CR)
{
    const char* INPUT = "\r";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\r' */
    TEST_EXPECT('\r' == INPUT[position]);
    TEST_EXPECT(13 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that SO is a NON-WS control character.
 */
TEST(scan_SO)
{
    const char* INPUT = "\16";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\16' */
    TEST_EXPECT('\16' == INPUT[position]);
    TEST_EXPECT(14 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that SI is a NON-WS control character.
 */
TEST(scan_SI)
{
    const char* INPUT = "\17";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\17' */
    TEST_EXPECT('\17' == INPUT[position]);
    TEST_EXPECT(15 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that DLE is a NON-WS control character.
 */
TEST(scan_DLE)
{
    const char* INPUT = "\20";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\20' */
    TEST_EXPECT('\20' == INPUT[position]);
    TEST_EXPECT(16 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that DC1 is a NON-WS control character.
 */
TEST(scan_DC1)
{
    const char* INPUT = "\21";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\21' */
    TEST_EXPECT('\21' == INPUT[position]);
    TEST_EXPECT(17 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that DC2 is a NON-WS control character.
 */
TEST(scan_DC2)
{
    const char* INPUT = "\22";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\22' */
    TEST_EXPECT('\22' == INPUT[position]);
    TEST_EXPECT(18 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that DC3 is a NON-WS control character.
 */
TEST(scan_DC3)
{
    const char* INPUT = "\23";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\23' */
    TEST_EXPECT('\23' == INPUT[position]);
    TEST_EXPECT(19 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that DC4 is a NON-WS control character.
 */
TEST(scan_DC4)
{
    const char* INPUT = "\24";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\24' */
    TEST_EXPECT('\24' == INPUT[position]);
    TEST_EXPECT(20 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that NAK is a NON-WS control character.
 */
TEST(scan_NAK)
{
    const char* INPUT = "\25";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\25' */
    TEST_EXPECT('\25' == INPUT[position]);
    TEST_EXPECT(21 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that SYN is a NON-WS control character.
 */
TEST(scan_SYN)
{
    const char* INPUT = "\26";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\26' */
    TEST_EXPECT('\26' == INPUT[position]);
    TEST_EXPECT(22 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that ETB is a NON-WS control character.
 */
TEST(scan_ETB)
{
    const char* INPUT = "\27";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\27' */
    TEST_EXPECT('\27' == INPUT[position]);
    TEST_EXPECT(23 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that CAN is a NON-WS control character.
 */
TEST(scan_CAN)
{
    const char* INPUT = "\30";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\30' */
    TEST_EXPECT('\30' == INPUT[position]);
    TEST_EXPECT(24 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that EM is a NON-WS control character.
 */
TEST(scan_EM)
{
    const char* INPUT = "\31";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\31' */
    TEST_EXPECT('\31' == INPUT[position]);
    TEST_EXPECT(25 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that SUB is a NON-WS control character.
 */
TEST(scan_SUB)
{
    const char* INPUT = "\32";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\32' */
    TEST_EXPECT('\32' == INPUT[position]);
    TEST_EXPECT(26 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that ESC is a NON-WS control character.
 */
TEST(scan_ESC)
{
    const char* INPUT = "\33";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\33' */
    TEST_EXPECT('\33' == INPUT[position]);
    TEST_EXPECT(27 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that FS is a NON-WS control character.
 */
TEST(scan_FS)
{
    const char* INPUT = "\34";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\34' */
    TEST_EXPECT('\34' == INPUT[position]);
    TEST_EXPECT(28 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that GS is a NON-WS control character.
 */
TEST(scan_GS)
{
    const char* INPUT = "\35";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\35' */
    TEST_EXPECT('\35' == INPUT[position]);
    TEST_EXPECT(29 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that RS is a NON-WS control character.
 */
TEST(scan_RS)
{
    const char* INPUT = "\36";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\36' */
    TEST_EXPECT('\36' == INPUT[position]);
    TEST_EXPECT(30 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that US is a NON-WS control character.
 */
TEST(scan_US)
{
    const char* INPUT = "\37";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be NON_WS_CONTROL. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\37' */
    TEST_EXPECT('\37' == INPUT[position]);
    TEST_EXPECT(31 == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that space is the whitespace primitive.
 */
TEST(scan_space)
{
    const char* INPUT = " ";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be WHITESPACE. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_WHITESPACE == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be ' ' */
    TEST_EXPECT(' ' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a 7-bit clean character can be scanned.
 */
TEST(scan_7bit)
{
    const char* INPUT = "z";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be U8_7BIT. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_U8_7BIT == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'z' */
    TEST_EXPECT('z' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a two-byte start character can be scanned.
 */
TEST(scan_2byte_start)
{
    const char* INPUT = "\xc7";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be U8_2BYTE_START. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_U8_2BYTE_START == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\xc7' */
    TEST_EXPECT('\xc7' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a three-byte start character can be scanned.
 */
TEST(scan_3byte_start)
{
    const char* INPUT = "\xe2";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be U8_3BYTE_START. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_U8_3BYTE_START == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\xe2' */
    TEST_EXPECT('\xe2' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a four-byte start character can be scanned.
 */
TEST(scan_4byte_start)
{
    const char* INPUT = "\xf1";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be U8_4BYTE_START. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_U8_4BYTE_START == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\xf1' */
    TEST_EXPECT('\xf1' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a continuation character can be scanned.
 */
TEST(scan_continuation)
{
    const char* INPUT = "\x8c";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be U8_CONTINUATION. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LL_U8_CONTINUATION == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\x8c' */
    TEST_EXPECT('\x8c' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a backslash character can be scanned.
 */
TEST(scan_backslash)
{
    const char* INPUT = R"(\)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be BACKSLASH. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_BACKSLASH == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '\\' */
    TEST_EXPECT('\\' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a forward slash character can be scanned.
 */
TEST(scan_forward_slash)
{
    const char* INPUT = R"(/)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be FORWARD_SLASH. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_FORWARD_SLASH == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be '/' */
    TEST_EXPECT('/' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that an n character can be scanned.
 */
TEST(scan_n)
{
    const char* INPUT = R"(n)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be LETTER_n. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LETTER_n == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'n' */
    TEST_EXPECT('n' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that an r character can be scanned.
 */
TEST(scan_r)
{
    const char* INPUT = R"(r)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be LETTER_r. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LETTER_r == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'r' */
    TEST_EXPECT('r' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a t character can be scanned.
 */
TEST(scan_t)
{
    const char* INPUT = R"(t)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be LETTER_t. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LETTER_t == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 't' */
    TEST_EXPECT('t' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that a u character can be scanned.
 */
TEST(scan_u)
{
    const char* INPUT = R"(u)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be LETTER_u. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LETTER_u == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'u' */
    TEST_EXPECT('u' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that the offset is NOT updated when lookahead is set to true.
 */
TEST(lookahead)
{
    const char* INPUT = R"(u)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, true));

    /* the primitive should be LETTER_u. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LETTER_u == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'u' */
    TEST_EXPECT('u' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(0 == offset);
}

/**
 * Verify that an l character can be scanned.
 */
TEST(scan_l)
{
    const char* INPUT = R"(l)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be LETTER_l. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LETTER_l == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 'l' */
    TEST_EXPECT('l' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that an s character can be scanned.
 */
TEST(scan_s)
{
    const char* INPUT = R"(s)";
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    /* scanning for a primitive should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcjson_scan_primitive(
                    &prim, &position, INPUT, size, &offset, false));

    /* the primitive should be LETTER_s. */
    TEST_EXPECT(VCJSON_LEXER_PRIM_LETTER_s == prim);
    /* the position should be 0. */
    TEST_EXPECT(0 == position);
    /* the value at this position should be 's' */
    TEST_EXPECT('s' == INPUT[position]);
    /* the offset should be 1. */
    TEST_EXPECT(1 == offset);
}

/**
 * Verify that all characters can be decoded.
 */
TEST(scan_all_characters)
{
    uint8_t INPUT[2] = { 0x20, 0x00 };
    size_t size = 1;
    int prim = 0;
    size_t position = 0;
    size_t offset = 0;

    for (uint16_t i = 0; i < 256; ++i)
    {
        INPUT[0] = i;
        offset = 0;

        /* scanning for a primitive should succeed. */
        TEST_ASSERT(
            STATUS_SUCCESS
                == vcjson_scan_primitive(
                        &prim, &position, (char*)INPUT, size, &offset, false));

        /* the position should be 0. */
        TEST_EXPECT(0 == position);
        /* the value at this position should be i */
        TEST_EXPECT((uint8_t)i == (uint8_t)INPUT[position]);
        /* the offset should be 1. */
        TEST_EXPECT(1 == offset);
    }
}
