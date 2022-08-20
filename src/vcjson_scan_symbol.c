/**
 * \file vcjson_scan_symbol.c
 *
 * \brief Scan the primitive symbols for the next symbol.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/* forward decls. */
static bool vcjson_is_digit(int symbol);
static bool vcjson_is_hexdigit(int symbol);
static bool vcjson_is_whitespace(int symbol);
static status 
vcjson_scan_string(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_true(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_false(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_null(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_maybe_decimal(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_number(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_negative_number(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_decimal(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_exponent(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_check_u8_2byte_seq(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_check_u8_3byte_seq(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_check_u8_4byte_seq(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_escape_sequence(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status 
vcjson_scan_unicode_escape_sequence(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset);
static status vcjson_u8_decode_2byte(
    uint32_t* codepoint, char byte1, char byte2);
static status vcjson_u8_decode_3byte(
    uint32_t* codepoint, char byte1, char byte2, char byte3);
static status vcjson_u8_decode_4byte(
    uint32_t* codepoint, char byte1, char byte2, char byte3, char byte4);
static status
vcjson_peek_termination_character(
    const char* input, size_t size, size_t* offset);

/**
 * Accept a token and assign to the given pointer, or fail with the given error
 * code.
 */
#define ACCEPT_ASSIGN_OR_FAIL(symbol_ptr, test, error_code) \
    retval = \
        vcjson_scan_primitive( \
            (symbol_ptr), endpos, input, size, offset, false); \
    do \
    { \
        if (STATUS_SUCCESS != retval) \
        { \
            return retval; \
        } \
        \
        if (! (test)) \
        { \
            *startpos = *endpos; \
            return (error_code); \
        } \
    } while (0)

/**
 * Accept a token or fail with the given error code.
 */
#define ACCEPT_OR_FAIL(test, error_code) \
    ACCEPT_ASSIGN_OR_FAIL(&prim, (test), (error_code))

/**
 * \brief Attempt to scan a buffer for the next symbol.
 *
 * \note This parse function sets a primitive as well as the buffer position.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_scan_symbol(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* scan for the next primitive symbol, skipping over whitespace. */
    do
    {
        retval =
            vcjson_scan_primitive(&prim, startpos, input, size, offset, false);
        if (STATUS_SUCCESS != retval)
        {
            *endpos = *startpos;
            goto done;
        }
    } while (vcjson_is_whitespace(prim));

    /* decode this symbol. */
    switch (prim)
    {
        /* handle simple symbols. */
        case VCJSON_LEXER_SYMBOL_SPECIAL_EOF:
        case VCJSON_LEXER_PRIM_LEFT_BRACKET:
        case VCJSON_LEXER_PRIM_RIGHT_BRACKET:
        case VCJSON_LEXER_PRIM_LEFT_BRACE:
        case VCJSON_LEXER_PRIM_RIGHT_BRACE:
        case VCJSON_LEXER_PRIM_COLON:
        case VCJSON_LEXER_PRIM_COMMA:
            *endpos = *startpos;
            *symbol = prim;
            break;

        /* handle the start of a string. */
        case VCJSON_LEXER_PRIM_QUOTE:
            retval =
                vcjson_scan_string(
                    symbol, startpos, endpos, input, size, offset);
            goto done;

        /* handle a possible true. */
        case VCJSON_LEXER_PRIM_LETTER_t:
            retval =
                vcjson_scan_true(
                    symbol, startpos, endpos, input, size, offset);
            goto done;

        /* handle a possible false. */
        case VCJSON_LEXER_PRIM_HEX_f:
            retval =
                vcjson_scan_false(
                    symbol, startpos, endpos, input, size, offset);
            goto done;

        /* handle a possible null. */
        case VCJSON_LEXER_PRIM_LETTER_n:
            retval =
                vcjson_scan_null(
                    symbol, startpos, endpos, input, size, offset);
            goto done;

        /* handle the start of a negative number. */
        case VCJSON_LEXER_PRIM_MINUS:
            *endpos = *startpos;
            retval =
                vcjson_scan_negative_number(
                    symbol, startpos, endpos, input, size, offset);
            goto done;

        /* handle the start of a possible decimal number. */
        case VCJSON_LEXER_PRIM_DIGIT_0:
            retval =
                vcjson_scan_maybe_decimal(
                    symbol, startpos, endpos, input, size, offset);
            goto done;

        /* handle the start of a possible number. */
        case VCJSON_LEXER_PRIM_DIGIT_1:
        case VCJSON_LEXER_PRIM_DIGIT_2:
        case VCJSON_LEXER_PRIM_DIGIT_3:
        case VCJSON_LEXER_PRIM_DIGIT_4:
        case VCJSON_LEXER_PRIM_DIGIT_5:
        case VCJSON_LEXER_PRIM_DIGIT_6:
        case VCJSON_LEXER_PRIM_DIGIT_7:
        case VCJSON_LEXER_PRIM_DIGIT_8:
        case VCJSON_LEXER_PRIM_DIGIT_9:
            *endpos = *startpos;
            retval =
                vcjson_scan_number(
                    symbol, startpos, endpos, input, size, offset);
            goto done;

        default:
            *endpos = *startpos;
            retval = ERROR_VCJSON_SCAN;
            goto done;
    }

    /* success. */
    retval = STATUS_SUCCESS;
    goto done;

done:
    return retval;
}

/**
 * \brief Check the symbol to see if it is whitespace.
 *
 * \param symbol        The symbol to check.
 *
 * \returns true if whitespace and false otherwise.
 */
static bool vcjson_is_whitespace(int symbol)
{
    switch (symbol)
    {
        case VCJSON_LEXER_PRIM_LL_WHITESPACE:
        case VCJSON_LEXER_PRIM_LL_WS_CONTROL:
            return true;

        default:
            return false;
    }
}

/**
 * \brief Check the symbol to see if it is a hex digit.
 *
 * \param symbol        The symbol to check.
 *
 * \returns true if the symbol is a hex digit and false otherwise.
 */
static bool vcjson_is_hexdigit(int symbol)
{
    switch (symbol)
    {
        case VCJSON_LEXER_PRIM_DIGIT_0:
        case VCJSON_LEXER_PRIM_DIGIT_1:
        case VCJSON_LEXER_PRIM_DIGIT_2:
        case VCJSON_LEXER_PRIM_DIGIT_3:
        case VCJSON_LEXER_PRIM_DIGIT_4:
        case VCJSON_LEXER_PRIM_DIGIT_5:
        case VCJSON_LEXER_PRIM_DIGIT_6:
        case VCJSON_LEXER_PRIM_DIGIT_7:
        case VCJSON_LEXER_PRIM_DIGIT_8:
        case VCJSON_LEXER_PRIM_DIGIT_9:
        case VCJSON_LEXER_PRIM_HEX_a:
        case VCJSON_LEXER_PRIM_HEX_A:
        case VCJSON_LEXER_PRIM_HEX_b:
        case VCJSON_LEXER_PRIM_HEX_B:
        case VCJSON_LEXER_PRIM_HEX_c:
        case VCJSON_LEXER_PRIM_HEX_C:
        case VCJSON_LEXER_PRIM_HEX_d:
        case VCJSON_LEXER_PRIM_HEX_D:
        case VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e:
        case VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_E:
        case VCJSON_LEXER_PRIM_HEX_f:
        case VCJSON_LEXER_PRIM_HEX_F:
            return true;

        default:
            return false;
    }
}

/**
 * \brief Check the symbol to see if it is a digit.
 *
 * \param symbol        The symbol to check.
 *
 * \returns true if the symbol is a digit and false otherwise.
 */
static bool vcjson_is_digit(int symbol)
{
    switch (symbol)
    {
        case VCJSON_LEXER_PRIM_DIGIT_0:
        case VCJSON_LEXER_PRIM_DIGIT_1:
        case VCJSON_LEXER_PRIM_DIGIT_2:
        case VCJSON_LEXER_PRIM_DIGIT_3:
        case VCJSON_LEXER_PRIM_DIGIT_4:
        case VCJSON_LEXER_PRIM_DIGIT_5:
        case VCJSON_LEXER_PRIM_DIGIT_6:
        case VCJSON_LEXER_PRIM_DIGIT_7:
        case VCJSON_LEXER_PRIM_DIGIT_8:
        case VCJSON_LEXER_PRIM_DIGIT_9:
            return true;

        default:
            return false;
    }
}

/**
 * \brief Attempt to scan a buffer for a string.
 *
 * \note This scanner assumes that the first quote has already been consumed,
 * and that both offset and startpos have been updated to reflect this.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
vcjson_scan_string(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    bool end_of_string = false;
    int prim;
    size_t subseq;

    /* scan over string. */
    do
    {
        retval =
            vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* decode primitive. */
        switch (prim)
        {
            /* a quote ends the string. */
            case VCJSON_LEXER_PRIM_QUOTE:
                end_of_string = true;
                break;

            /* EOF before we reach the end of the string is an error. */
            case VCJSON_LEXER_SYMBOL_SPECIAL_EOF:
                *startpos = *endpos;
                retval = ERROR_VCJSON_SCAN_53098ec5_2c31_46f2_8b6c_17c0674708b5;
                goto done;

            /* an unpaired continuation byte is invalid. */
            case VCJSON_LEXER_PRIM_LL_U8_CONTINUATION:
                *startpos = *endpos;
                retval = ERROR_VCJSON_SCAN_f44c9426_32bc_4191_9e80_a508171a6d41;
                goto done;

            /* Verify that a two-byte UTF-8 sequence ends correctly. */
            case VCJSON_LEXER_PRIM_LL_U8_2BYTE_START:
                subseq = *endpos;
                retval =
                    vcjson_scan_check_u8_2byte_seq(
                        symbol, &subseq, endpos, input, size, offset);
                if (STATUS_SUCCESS != retval)
                {
                    *startpos = subseq;
                    goto done;
                }
                break;

            /* Verify that a three-byte UTF-8 sequence ends correctly. */
            case VCJSON_LEXER_PRIM_LL_U8_3BYTE_START:
                subseq = *endpos;
                retval =
                    vcjson_scan_check_u8_3byte_seq(
                        symbol, &subseq, endpos, input, size, offset);
                if (STATUS_SUCCESS != retval)
                {
                    *startpos = subseq;
                    goto done;
                }
                break;

            /* Verify that a four-byte UTF-8 sequence ends correctly. */
            case VCJSON_LEXER_PRIM_LL_U8_4BYTE_START:
                subseq = *endpos;
                retval =
                    vcjson_scan_check_u8_4byte_seq(
                        symbol, &subseq, endpos, input, size, offset);
                if (STATUS_SUCCESS != retval)
                {
                    *startpos = subseq;
                    goto done;
                }
                break;

            /* a literal whitespace control character is invalid. */
            case VCJSON_LEXER_PRIM_LL_WS_CONTROL:
                *startpos = *endpos;
                retval = ERROR_VCJSON_SCAN_903c7867_9325_4576_85ac_3e312735def9;
                goto done;

            /* a literal control character is invalid. */
            case VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL:
                *startpos = *endpos;
                retval = ERROR_VCJSON_SCAN_e08745b4_8269_4c1d_bebe_474170354990;
                goto done;

            /* a backslash starts a control character sequence. */
            case VCJSON_LEXER_PRIM_BACKSLASH:
                subseq = *endpos;
                retval =
                    vcjson_scan_escape_sequence(
                        symbol, &subseq, endpos, input, size, offset);
                if (STATUS_SUCCESS != retval)
                {
                    *startpos = subseq;
                    goto done;
                }
                break;
        }
    } while (!end_of_string);

    /* if we make it this far, then we've successfully scanned a string. */
    *symbol = VCJSON_LEXER_SYMBOL_STRING;
    retval = STATUS_SUCCESS;
    goto done;

done:
    return retval;
}

/**
 * \brief Verify that a two-byte UTF-8 sequence is valid.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_check_u8_2byte_seq(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* the next byte is a continuation byte. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_LL_U8_CONTINUATION == prim,
        ERROR_VCJSON_SCAN_40221afc_d49e_404e_8b18_1be7e48a2ecf);

    /* convert the two-byte sequence into a codepoint. */
    uint32_t codepoint;
    retval =
        vcjson_u8_decode_2byte(&codepoint, input[*startpos], input[*endpos]);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* success. */
    *symbol = VCJSON_LEXER_SYMBOL_LL_CODEPOINT;
    return STATUS_SUCCESS;
}

/**
 * \brief Decode a UTF-8 two-byte sequence.
 *
 * \param codepoint Pointer to receive the codepoint.
 * \param byte1     The first byte of this sequence.
 * \param byte2     The second byte of this sequence.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_u8_decode_2byte(
    uint32_t* codepoint, char byte1, char byte2)
{
    uint32_t decoded =
        ((uint32_t)(byte1 & 0x1F)) << 6 | ((uint32_t)(byte2 & 0x3F));

    /* check for an overlong codepoint. */
    if (decoded < 0x80)
    {
        return ERROR_VCJSON_SCAN_8a7564a9_b682_4ea0_88bc_629395331c5f;
    }

    /* otherwise, the decode was successful. */
    *codepoint = decoded;
    return STATUS_SUCCESS;
}

/**
 * \brief Verify that a three-byte UTF-8 sequence is valid.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_check_u8_3byte_seq(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim1, prim2;

    /* the second byte is a continuation byte. */
    ACCEPT_ASSIGN_OR_FAIL(
        &prim1, prim1 == VCJSON_LEXER_PRIM_LL_U8_CONTINUATION,
        ERROR_VCJSON_SCAN_3371878e_c9c8_44b8_be57_60d9ace6bd4f);

    /* the third byte is a continuation byte. */
    ACCEPT_ASSIGN_OR_FAIL(
        &prim2, prim2 == VCJSON_LEXER_PRIM_LL_U8_CONTINUATION,
        ERROR_VCJSON_SCAN_3371878e_c9c8_44b8_be57_60d9ace6bd4f);

    /* convert the three-byte sequence into a codepoint. */
    uint32_t codepoint;
    retval =
        vcjson_u8_decode_3byte(
            &codepoint, input[*startpos], input[*startpos + 1], input[*endpos]);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* success. */
    *symbol = VCJSON_LEXER_SYMBOL_LL_CODEPOINT;
    return STATUS_SUCCESS;
}

/**
 * \brief Decode a UTF-8 three-byte sequence.
 *
 * \param codepoint Pointer to receive the codepoint.
 * \param byte1     The first byte of this sequence.
 * \param byte2     The second byte of this sequence.
 * \param byte3     The third byte of this sequence.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_u8_decode_3byte(
    uint32_t* codepoint, char byte1, char byte2, char byte3)
{
    uint32_t decoded =
        ((uint32_t)(byte1 & 0x1F)) << 12
      | ((uint32_t)(byte2 & 0x3F) << 6)
      | ((uint32_t)(byte3 & 0x3F));

    /* check for an overlong codepoint. */
    if (decoded < 0x800)
    {
        return ERROR_VCJSON_SCAN_696f7da7_6349_4356_827b_01fba3599d4c;
    }

    /* check for a codepoint in the surrogate pair range. */
    if (decoded >= 0xD800 && decoded <= 0xDFFF)
    {
        return ERROR_VCJSON_SCAN_d64622e3_3077_46e2_84a4_d21b9a6dc02f;
    }

    /* otherwise, the decode was successful. */
    *codepoint = decoded;
    return STATUS_SUCCESS;
}

/**
 * \brief Verify that a four-byte UTF-8 sequence is valid.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_check_u8_4byte_seq(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim1, prim2, prim3;

    /* the second byte is a continuation byte. */
    ACCEPT_ASSIGN_OR_FAIL(
        &prim1, prim1 == VCJSON_LEXER_PRIM_LL_U8_CONTINUATION,
        ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8);

    /* the third byte is a continuation byte. */
    ACCEPT_ASSIGN_OR_FAIL(
        &prim2, prim2 == VCJSON_LEXER_PRIM_LL_U8_CONTINUATION,
        ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8);

    /* the fourth byte is a continuation byte. */
    ACCEPT_ASSIGN_OR_FAIL(
        &prim3, prim3 == VCJSON_LEXER_PRIM_LL_U8_CONTINUATION,
        ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8);

    /* convert the four-byte sequence into a codepoint. */
    uint32_t codepoint;
    retval =
        vcjson_u8_decode_4byte(
            &codepoint, input[*startpos], input[*startpos + 1],
            input[*startpos + 2], input[*endpos]);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* success. */
    *symbol = VCJSON_LEXER_SYMBOL_LL_CODEPOINT;
    return STATUS_SUCCESS;
}

/**
 * \brief Decode a UTF-8 four-byte sequence.
 *
 * \param codepoint Pointer to receive the codepoint.
 * \param byte1     The first byte of this sequence.
 * \param byte2     The second byte of this sequence.
 * \param byte3     The third byte of this sequence.
 * \param byte4     The fourth byte of this sequence.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status vcjson_u8_decode_4byte(
    uint32_t* codepoint, char byte1, char byte2, char byte3, char byte4)
{
    uint32_t decoded =
        ((uint32_t)(byte1 & 0x07) << 18)
      | ((uint32_t)(byte2 & 0x3F) << 12)
      | ((uint32_t)(byte3 & 0x3F) <<  6)
      | ((uint32_t)(byte4 & 0x3F));

    /* check for an overlong codepoint. */
    if (decoded < 0x10000)
    {
        return ERROR_VCJSON_SCAN_dbf3f552_c055_4616_96bf_c48f17893162;
    }

    /* check for an out-of-range codepoint. */
    if (decoded > 0x10FFFF)
    {
        return ERROR_VCJSON_SCAN_3673de25_4603_4492_a25a_31752e9395fc;
    }

    /* otherwise, the decode was successful. */
    *codepoint = decoded;
    return STATUS_SUCCESS;
}

/**
 * \brief Scan an escape sequence.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_escape_sequence(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;
    size_t subseq;

    /* read the next character. */
    retval =
        vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* decode this character. */
    switch (prim)
    {
        /* an EOF means that the escape sequence is partial. */
        case VCJSON_LEXER_SYMBOL_SPECIAL_EOF:
            *startpos = *endpos;
            return ERROR_VCJSON_SCAN_89b93262_852c_4ab7_a41c_2da08a73a850;

        /* a quote can be escaped. */
        case VCJSON_LEXER_PRIM_QUOTE:
        /* a backslash can be escaped. */
        case VCJSON_LEXER_PRIM_BACKSLASH:
        /* a forward slash can be escaped. */
        case VCJSON_LEXER_PRIM_FORWARD_SLASH:
        /* a backspace can be escaped. */
        case VCJSON_LEXER_PRIM_HEX_b:
        /* a form feed can be escaped. */
        case VCJSON_LEXER_PRIM_HEX_f:
        /* a line feed can be escaped. */
        case VCJSON_LEXER_PRIM_LETTER_n:
        /* a carriage return can be escaped. */
        case VCJSON_LEXER_PRIM_LETTER_r:
        /* a horizontal tab can be escaped. */
        case VCJSON_LEXER_PRIM_LETTER_t:
            *symbol = VCJSON_LEXER_SYMBOL_LL_ESCAPE;
            return STATUS_SUCCESS;

        /* a unicode escape sequence can be accepted. */
        case VCJSON_LEXER_PRIM_LETTER_u:
            subseq = *endpos;
            retval = vcjson_scan_unicode_escape_sequence(
                symbol, &subseq, endpos, input, size, offset);
            if (STATUS_SUCCESS != retval)
            {
                *startpos = subseq;
            }
            return retval;

        /* any other escape sequence is invalid. */
        default:
            *startpos = *endpos;
            return ERROR_VCJSON_SCAN_65b96e7e_25c7_4f2a_9c8d_bce126776faa;
    }
}

/**
 * \brief Scan a unicode escape sequence.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_unicode_escape_sequence(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* accept the first hex digit. */
    ACCEPT_OR_FAIL(
        vcjson_is_hexdigit(prim), 
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2);

    /* accept the second hex digit. */
    ACCEPT_OR_FAIL(
        vcjson_is_hexdigit(prim), 
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2);

    /* accept the third hex digit. */
    ACCEPT_OR_FAIL(
        vcjson_is_hexdigit(prim), 
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2);

    /* accept the fourth hex digit. */
    ACCEPT_OR_FAIL(
        vcjson_is_hexdigit(prim), 
        ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2);

    /* success. */
    *symbol = VCJSON_LEXER_SYMBOL_LL_ESCAPE;
    return STATUS_SUCCESS;
}

/**
 * \brief Attempt to scan a buffer for true.
 *
 * \note This scanner assumes that the first letter has already been consumed,
 * and that both offset and startpos have been updated to reflect this.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
vcjson_scan_true(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* accept an 'r'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_LETTER_r == prim,
        ERROR_VCJSON_SCAN_c48ee2cb_30dc_4264_90f8_c8545f131429);

    /* accept a 'u'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_LETTER_u == prim,
        ERROR_VCJSON_SCAN_b71d4bff_342e_45ce_b1d5_e77f472ebc95);

    /* accept an 'e'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e == prim,
        ERROR_VCJSON_SCAN_ddb74f50_c250_463f_ad04_7fb6a181790d);

    /* look ahead to the next character. */
    retval = vcjson_scan_primitive(&prim, endpos, input, size, offset, true);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* make sure that this token has terminated. */
    retval = vcjson_peek_termination_character(input, size, offset);
    if (STATUS_SUCCESS != retval)
    {
        *endpos += 1;
        *startpos = *endpos;
        return retval;
    }

    /* success. */
    *symbol = VCJSON_LEXER_SYMBOL_TRUE;
    return STATUS_SUCCESS;
}

/**
 * \brief Attempt to scan a buffer for false.
 *
 * \note This scanner assumes that the first letter has already been consumed,
 * and that both offset and startpos have been updated to reflect this.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
vcjson_scan_false(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* accept an 'a'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_HEX_a == prim,
        ERROR_VCJSON_SCAN_9b5a8590_05d8_4c88_a45a_48b4ebf1ce3c);

    /* accept an 'l'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_LETTER_l == prim,
        ERROR_VCJSON_SCAN_19478855_7462_4d89_a9c9_ff500d4328c2);

    /* accept an 's'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_LETTER_s == prim,
        ERROR_VCJSON_SCAN_2783f9f0_77a0_4b4c_b561_8dc932f89ea3);

    /* accept an 'e'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e == prim,
        ERROR_VCJSON_SCAN_83ad3fa4_4479_4fc0_9f28_912c2c619473);

    /* make sure that this token has terminated. */
    retval = vcjson_peek_termination_character(input, size, offset);
    if (STATUS_SUCCESS != retval)
    {
        *endpos += 1;
        *startpos = *endpos;
        return retval;
    }

    /* success. */
    *symbol = VCJSON_LEXER_SYMBOL_FALSE;
    return STATUS_SUCCESS;
}

/**
 * \brief Peek for a termination character.
 *
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
vcjson_peek_termination_character(
    const char* input, size_t size, size_t* offset)
{
    status retval;
    int prim;
    size_t position;

    /* look ahead to the next character. */
    retval = vcjson_scan_primitive(&prim, &position, input, size, offset, true);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that this next character is a token terminator. */
    switch (prim)
    {
        case VCJSON_LEXER_PRIM_COMMA:
        case VCJSON_LEXER_PRIM_RIGHT_BRACKET:
        case VCJSON_LEXER_PRIM_RIGHT_BRACE:
        case VCJSON_LEXER_PRIM_LL_WS_CONTROL:
        case VCJSON_LEXER_PRIM_LL_WHITESPACE:
        case VCJSON_LEXER_SYMBOL_SPECIAL_EOF:
            /* success. */
            return STATUS_SUCCESS;

        default:
            return ERROR_VCJSON_SCAN_299d80db_2eec_4ed3_9717_1b3ecd188c4c;
    }
}

/**
 * \brief Attempt to scan a buffer for null.
 *
 * \note This scanner assumes that the first letter has already been consumed,
 * and that both offset and startpos have been updated to reflect this.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status
vcjson_scan_null(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* accept a 'u'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_LETTER_u == prim,
        ERROR_VCJSON_SCAN_21dcc558_3dc4_47dc_9ea2_e8d7434cdf4a);

    /* accept an 'l'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_LETTER_l == prim,
        ERROR_VCJSON_SCAN_2519e212_7d80_41ab_af09_72989c6f5647);

    /* accept an 'l'. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_LETTER_l == prim,
        ERROR_VCJSON_SCAN_2519e212_7d80_41ab_af09_72989c6f5647);

    /* make sure that this token has terminated. */
    retval = vcjson_peek_termination_character(input, size, offset);
    if (STATUS_SUCCESS != retval)
    {
        *endpos += 1;
        *startpos = *endpos;
        return retval;
    }

    /* success. */
    *symbol = VCJSON_LEXER_SYMBOL_NULL;
    return STATUS_SUCCESS;
}

/**
 * \brief Scan for an optional decimal portion of a number.
 *
 * \note This scanner assumes that the first digits have already been consumed,
 * and that both offset and startpos have been updated to reflect this.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_maybe_decimal(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* peek the next primitive in the sequence. */
    retval =
        vcjson_scan_primitive(
            &prim, endpos, input, size, offset, true);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* is this a decimal point? */
    if (VCJSON_LEXER_PRIM_DOT == prim)
    {
        /* finish in the scan decimal method. */
        return
            vcjson_scan_decimal(
                symbol, startpos, endpos, input, size, offset);
    }
    else if (
        VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e == prim
     || VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_E == prim)
    {
        /* finish in the scan exponent method. */
        return
            vcjson_scan_exponent(
                symbol, startpos, endpos, input, size, offset);
    }
    else
    {
        /* if not, it must be a valid termination character. */
        retval = vcjson_peek_termination_character(input, size, offset);
        if (STATUS_SUCCESS != retval)
        {
            *startpos += 1;
            *endpos = *startpos;
            return retval;
        }
        else
        {
            /* success. */
            *endpos = *offset - 1;
            *symbol = VCJSON_LEXER_SYMBOL_NUMBER;
            return STATUS_SUCCESS;
        }
    }
}

/**
 * \brief Scan the decimal portion of a number.
 *
 * \note This scanner assumes that the decimal point been peeked but not
 * accepted.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_decimal(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* accept the decimal point. */
    ACCEPT_OR_FAIL(
        VCJSON_LEXER_PRIM_DOT == prim,
        ERROR_VCJSON_SCAN_31f9107a_1bea_44a6_84ab_e507c8fa4b6e);

    /* get the next primitive. */
    retval =
        vcjson_scan_primitive(
            &prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* there MUST be at least one digit after the decimal point. */
    if (!vcjson_is_digit(prim))
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_64adb94e_5295_49e6_ba62_44497c8cd58f;
    }

    for (;;)
    {
        /* peek the next primitive in the sequence. */
        retval =
            vcjson_scan_primitive(
                &prim, endpos, input, size, offset, true);
        if (STATUS_SUCCESS != retval)
        {
            return retval;
        }

        /* is this a digit? */
        if (vcjson_is_digit(prim))
        {
            /* accept the digit as input. */
            ACCEPT_OR_FAIL(
                vcjson_is_digit(prim),
                ERROR_VCJSON_SCAN_31f9107a_1bea_44a6_84ab_e507c8fa4b6e);
        }
        /* is this an exponent? */
        else if (
            VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e == prim
         || VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_E == prim)
        {
            return
                vcjson_scan_exponent(
                    symbol, startpos, endpos, input, size, offset);
        }
        else
        {
            /* if not, it must be a valid termination character. */
            retval = vcjson_peek_termination_character(input, size, offset);
            if (STATUS_SUCCESS != retval)
            {
                *endpos += 1;
                *startpos = *endpos;
                return retval;
            }
            else
            {
                /* success. */
                *symbol = VCJSON_LEXER_SYMBOL_NUMBER;
                return STATUS_SUCCESS;
            }
        }
    }
}

/**
 * \brief Scan for an exponent.
 *
 * \note This scanner assumes that the exponent has been discovered via peek but
 * has not yet been consumed.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_exponent(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* accept the exponent delimiter as input. */
    ACCEPT_OR_FAIL(
        (VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e == prim
         || VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_E == prim),
        ERROR_VCJSON_SCAN_96c5bf68_6cff_490d_a7f9_d5b082116050);

    /* peek the next primitive in the sequence. */
    retval =
        vcjson_scan_primitive(
            &prim, endpos, input, size, offset, true);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* if it's a sign, accept it. */
    if (VCJSON_LEXER_PRIM_PLUS == prim || VCJSON_LEXER_PRIM_MINUS == prim)
    {
        /* accept the exponent delimiter as input. */
        ACCEPT_OR_FAIL(
            (VCJSON_LEXER_PRIM_PLUS == prim || VCJSON_LEXER_PRIM_MINUS == prim),
            ERROR_VCJSON_SCAN_1b6d6898_f81d_44b3_9c16_0c5a6e4b5a1c);
    }

    /* accept a digit as input. */
    ACCEPT_OR_FAIL(
        vcjson_is_digit(prim),
        ERROR_VCJSON_SCAN_1b6d6898_f81d_44b3_9c16_0c5a6e4b5a1c);

    /* while there are more digits, accept them. */
    for (;;)
    {
        /* peek the next primitive in the sequence. */
        retval =
            vcjson_scan_primitive(
                &prim, endpos, input, size, offset, true);
        if (STATUS_SUCCESS != retval)
        {
            return retval;
        }

        /* is this a digit? */
        if (vcjson_is_digit(prim))
        {
            /* accept the digit as input. */
            ACCEPT_OR_FAIL(
                vcjson_is_digit(prim),
                ERROR_VCJSON_SCAN_31f9107a_1bea_44a6_84ab_e507c8fa4b6e);
        }
        else
        {
            /* if not, it must be a valid termination character. */
            retval = vcjson_peek_termination_character(input, size, offset);
            if (STATUS_SUCCESS != retval)
            {
                *endpos += 1;
                *startpos = *endpos;
                return retval;
            }
            else
            {
                /* success. */
                *symbol = VCJSON_LEXER_SYMBOL_NUMBER;
                return STATUS_SUCCESS;
            }
        }
    }
}

/**
 * \brief Scan for a number.
 *
 * \note This scanner assumes that the first digit of the number (1-9) has
 * already been accepted.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_number(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    for (;;)
    {
        /* peek the next primitive in the sequence. */
        retval =
            vcjson_scan_primitive(
                &prim, endpos, input, size, offset, true);
        if (STATUS_SUCCESS != retval)
        {
            return retval;
        }

        /* is this a digit? */
        if (vcjson_is_digit(prim))
        {
            /* accept the digit as input. */
            ACCEPT_OR_FAIL(
                vcjson_is_digit(prim),
                ERROR_VCJSON_SCAN_31f9107a_1bea_44a6_84ab_e507c8fa4b6e);
        }
        /* is this a decimal place? */
        else if (VCJSON_LEXER_PRIM_DOT == prim)
        {
            return
                vcjson_scan_decimal(
                    symbol, startpos, endpos, input, size, offset);
        }
        /* is this an exponent? */
        else if (
            VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e == prim
         || VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_E == prim)
        {
            return
                vcjson_scan_exponent(
                    symbol, startpos, endpos, input, size, offset);
        }
        else
        {
            /* if not, it must be a valid termination character. */
            retval = vcjson_peek_termination_character(input, size, offset);
            if (STATUS_SUCCESS != retval)
            {
                *endpos += 1;
                *startpos = *endpos;
                return retval;
            }
            else
            {
                /* success. */
                *symbol = VCJSON_LEXER_SYMBOL_NUMBER;
                return STATUS_SUCCESS;
            }
        }
    }
}

/**
 * \brief Scan for a negative number.
 *
 * \note This scanner assumes that the negative sign has already been accepted.
 *
 * \param symbol        Pointer to the symbol value to set.
 * \param startpos      Pointer to be set with the start position of this
 *                      symbol.
 * \param endpos        Pointer to be set with the end position of this symbol.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status 
vcjson_scan_negative_number(
    int* symbol, size_t* startpos, size_t* endpos, const char* input,
    size_t size, size_t* offset)
{
    status retval;
    int prim;

    /* scan for the first digit in the number. */
    retval =
        vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        *startpos = *endpos;
        return retval;
    }

    /* decode this digit. */
    switch (prim)
    {
        /* handle the start of a possible decimal number. */
        case VCJSON_LEXER_PRIM_DIGIT_0:
            return
                vcjson_scan_maybe_decimal(
                    symbol, startpos, endpos, input, size, offset);

        /* handle the start of a possible number. */
        case VCJSON_LEXER_PRIM_DIGIT_1:
        case VCJSON_LEXER_PRIM_DIGIT_2:
        case VCJSON_LEXER_PRIM_DIGIT_3:
        case VCJSON_LEXER_PRIM_DIGIT_4:
        case VCJSON_LEXER_PRIM_DIGIT_5:
        case VCJSON_LEXER_PRIM_DIGIT_6:
        case VCJSON_LEXER_PRIM_DIGIT_7:
        case VCJSON_LEXER_PRIM_DIGIT_8:
        case VCJSON_LEXER_PRIM_DIGIT_9:
            return
                vcjson_scan_number(
                    symbol, startpos, endpos, input, size, offset);

        default:
            *startpos = *endpos;
            return ERROR_VCJSON_SCAN_9c0be0f4_2ac5_4713_9279_c90b672c0f5b;
    }
}
