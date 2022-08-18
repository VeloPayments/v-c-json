/**
 * \file vcjson_scan_symbol.c
 *
 * \brief Scan the primitive symbols for the next symbol.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/* forward decls. */
static bool vcjson_is_whitespace(int symbol);
static bool vcjson_is_hexdigit(int symbol);
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
 * \brief Attempt to scan a buffer for a string.
 *
 * \note This scanner assumes that the first quote has already been consumed,
 * and that both offset and startpos have been update to reflect this.
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

    (void)startpos;

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

    /* read the next character. */
    retval =
        vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* it should be a continuation byte. */
    if (VCJSON_LEXER_PRIM_LL_U8_CONTINUATION != prim)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_40221afc_d49e_404e_8b18_1be7e48a2ecf;
    }

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

    /* read the second byte character. */
    retval =
        vcjson_scan_primitive(&prim1, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* it should be a continuation byte. */
    if (VCJSON_LEXER_PRIM_LL_U8_CONTINUATION != prim1)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_3371878e_c9c8_44b8_be57_60d9ace6bd4f;
    }

    /* read the third byte character. */
    retval =
        vcjson_scan_primitive(&prim2, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* it should be a continuation byte. */
    if (VCJSON_LEXER_PRIM_LL_U8_CONTINUATION != prim2)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_3371878e_c9c8_44b8_be57_60d9ace6bd4f;
    }

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

    /* read the second byte character. */
    retval =
        vcjson_scan_primitive(&prim1, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* it should be a continuation byte. */
    if (VCJSON_LEXER_PRIM_LL_U8_CONTINUATION != prim1)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8;
    }

    /* read the third byte character. */
    retval =
        vcjson_scan_primitive(&prim2, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* it should be a continuation byte. */
    if (VCJSON_LEXER_PRIM_LL_U8_CONTINUATION != prim2)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8;
    }

    /* read the fourth byte character. */
    retval =
        vcjson_scan_primitive(&prim3, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* it should be a continuation byte. */
    if (VCJSON_LEXER_PRIM_LL_U8_CONTINUATION != prim3)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_c85ab469_9208_4917_958f_a8c75758dbb8;
    }

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

    /* read the first hex digit. */
    retval =
        vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that it's a hex digit. */
    if (!vcjson_is_hexdigit(prim))
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2;
    }

    /* read the second hex digit. */
    retval =
        vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that it's a hex digit. */
    if (!vcjson_is_hexdigit(prim))
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2;
    }

    /* read the third hex digit. */
    retval =
        vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that it's a hex digit. */
    if (!vcjson_is_hexdigit(prim))
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2;
    }

    /* read the fourth hex digit. */
    retval =
        vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that it's a hex digit. */
    if (!vcjson_is_hexdigit(prim))
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_437e1025_7c3f_4a65_92d5_771930c7a3d2;
    }

    /* success. */
    *symbol = VCJSON_LEXER_SYMBOL_LL_ESCAPE;
    return STATUS_SUCCESS;
}

/**
 * \brief Attempt to scan a buffer for true.
 *
 * \note This scanner assumes that the first letter has already been consumed,
 * and that both offset and startpos have been update to reflect this.
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

    /* attempt to read the 'r'. */
    retval = vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that this is an 'r'. */
    if (VCJSON_LEXER_PRIM_LETTER_r != prim)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_c48ee2cb_30dc_4264_90f8_c8545f131429;
    }

    /* attempt to read the 'u'. */
    retval = vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that this is a 'u'. */
    if (VCJSON_LEXER_PRIM_LETTER_u != prim)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_b71d4bff_342e_45ce_b1d5_e77f472ebc95;
    }

    /* attempt to read the 'e'. */
    retval = vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that this is an 'e'. */
    if (VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e != prim)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_ddb74f50_c250_463f_ad04_7fb6a181790d;
    }

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
 * and that both offset and startpos have been update to reflect this.
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

    /* attempt to read the 'a'. */
    retval = vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that this is an 'a'. */
    if (VCJSON_LEXER_PRIM_HEX_a != prim)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_9b5a8590_05d8_4c88_a45a_48b4ebf1ce3c;
    }

    /* attempt to read the 'l'. */
    retval = vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that this is an 'l'. */
    if (VCJSON_LEXER_PRIM_LETTER_l != prim)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_19478855_7462_4d89_a9c9_ff500d4328c2;
    }

    /* attempt to read the 's'. */
    retval = vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that this is an 's'. */
    if (VCJSON_LEXER_PRIM_LETTER_s != prim)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_2783f9f0_77a0_4b4c_b561_8dc932f89ea3;
    }

    /* attempt to read the 'e'. */
    retval = vcjson_scan_primitive(&prim, endpos, input, size, offset, false);
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* verify that this is an 'e'. */
    if (VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e != prim)
    {
        *startpos = *endpos;
        return ERROR_VCJSON_SCAN_83ad3fa4_4479_4fc0_9f28_912c2c619473;
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
