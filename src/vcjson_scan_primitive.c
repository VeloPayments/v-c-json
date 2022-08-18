/**
 * \file vcjson_scan_primitive.c
 *
 * \brief Scan the input for a primitive.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

/**
 * \brief Attempt to scan a buffer for the next primitive symbol.
 *
 * \note This parse function sets a primitive as well as the buffer position.
 *
 * \param prim          Pointer to the primitive value to set.
 * \param position      Pointer to be set with the position of this primitive.
 * \param input         Pointer to the input buffer to scan.
 * \param size          The size of this input buffer.
 * \param offset        Pointer to the current offset, to be updated on success.
 * \param lookahead     Set to true if we are only looking ahead.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_scan_primitive(
    int* prim, size_t* position, const char* input, size_t size,
    size_t* offset, bool lookahead)
{
    (void)lookahead;

    /* are we at EOF? */
    if (*offset >= size)
    {
        *prim = VCJSON_LEXER_SYMBOL_SPECIAL_EOF;
        if (!lookahead)
        {
            *position = *offset;
        }
        return STATUS_SUCCESS;
    }

    /* get the next character in the buffer. */
    uint8_t ch = (uint8_t)input[*offset];

    /* TODO - add UTF-8 decoding. */

    /* decode this character. */
    switch (ch)
    {
        case ':': *prim = VCJSON_LEXER_PRIM_COLON;                      break;
        case ',': *prim = VCJSON_LEXER_PRIM_COMMA;                      break;
        case '0': *prim = VCJSON_LEXER_PRIM_DIGIT_0;                    break;
        case '1': *prim = VCJSON_LEXER_PRIM_DIGIT_1;                    break;
        case '2': *prim = VCJSON_LEXER_PRIM_DIGIT_2;                    break;
        case '3': *prim = VCJSON_LEXER_PRIM_DIGIT_3;                    break;
        case '4': *prim = VCJSON_LEXER_PRIM_DIGIT_4;                    break;
        case '5': *prim = VCJSON_LEXER_PRIM_DIGIT_5;                    break;
        case '6': *prim = VCJSON_LEXER_PRIM_DIGIT_6;                    break;
        case '7': *prim = VCJSON_LEXER_PRIM_DIGIT_7;                    break;
        case '8': *prim = VCJSON_LEXER_PRIM_DIGIT_8;                    break;
        case '9': *prim = VCJSON_LEXER_PRIM_DIGIT_9;                    break;
        case '.': *prim = VCJSON_LEXER_PRIM_DOT;                        break;
        case 'a': *prim = VCJSON_LEXER_PRIM_HEX_a;                      break;
        case 'A': *prim = VCJSON_LEXER_PRIM_HEX_A;                      break;
        case 'b': *prim = VCJSON_LEXER_PRIM_HEX_b;                      break;
        case 'B': *prim = VCJSON_LEXER_PRIM_HEX_B;                      break;
        case 'c': *prim = VCJSON_LEXER_PRIM_HEX_c;                      break;
        case 'C': *prim = VCJSON_LEXER_PRIM_HEX_C;                      break;
        case 'd': *prim = VCJSON_LEXER_PRIM_HEX_d;                      break;
        case 'D': *prim = VCJSON_LEXER_PRIM_HEX_D;                      break;
        case 'e': *prim = VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e;          break;
        case 'E': *prim = VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_E;          break;
        case 'f': *prim = VCJSON_LEXER_PRIM_HEX_f;                      break;
        case 'F': *prim = VCJSON_LEXER_PRIM_HEX_F;                      break;
        case 'n': *prim = VCJSON_LEXER_PRIM_LETTER_n;                   break;
        case 'r': *prim = VCJSON_LEXER_PRIM_LETTER_r;                   break;
        case 't': *prim = VCJSON_LEXER_PRIM_LETTER_t;                   break;
        case 'u': *prim = VCJSON_LEXER_PRIM_LETTER_u;                   break;
        case '{': *prim = VCJSON_LEXER_PRIM_LEFT_BRACE;                 break;
        case '[': *prim = VCJSON_LEXER_PRIM_LEFT_BRACKET;               break;
        case '-': *prim = VCJSON_LEXER_PRIM_MINUS;                      break;
        case '+': *prim = VCJSON_LEXER_PRIM_PLUS;                       break;
        case '"': *prim = VCJSON_LEXER_PRIM_QUOTE;                      break;
        case '}': *prim = VCJSON_LEXER_PRIM_RIGHT_BRACE;                break;
        case ']': *prim = VCJSON_LEXER_PRIM_RIGHT_BRACKET;              break;
        case '\\': *prim = VCJSON_LEXER_PRIM_BACKSLASH;                 break;
        case '/': *prim = VCJSON_LEXER_PRIM_FORWARD_SLASH;              break;
        case '\0': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;         break;
        case '\1': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;         break;
        case '\2': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;         break;
        case '\3': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;         break;
        case '\4': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;         break;
        case '\5': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;         break;
        case '\6': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;         break;
        case '\7': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;         break;
        case '\10': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\t': *prim = VCJSON_LEXER_PRIM_LL_WS_CONTROL;             break;
        case '\n': *prim = VCJSON_LEXER_PRIM_LL_WS_CONTROL;             break;
        case '\13': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\14': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\r': *prim = VCJSON_LEXER_PRIM_LL_WS_CONTROL;             break;
        case '\16': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\17': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\20': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\21': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\22': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\23': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\24': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\25': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\26': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\27': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\30': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\31': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\32': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\33': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\34': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\35': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\36': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case '\37': *prim = VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL;        break;
        case ' ': *prim = VCJSON_LEXER_PRIM_LL_WHITESPACE;              break;
        default:
            /* check to see if the character is 7-bit clean. */
            if (0 == (ch & 0x80))
            {
                *prim = VCJSON_LEXER_PRIM_LL_U8_7BIT;
            }
            /* is it a 4-byte start? */
            else if (0xf0 == (ch & 0xf0))
            {
                *prim = VCJSON_LEXER_PRIM_LL_U8_4BYTE_START;
            }
            /* is it a 3-byte start? */
            else if (0xe0 == (ch & 0xe0))
            {
                *prim = VCJSON_LEXER_PRIM_LL_U8_3BYTE_START;
            }
            /* is it a 2-byte start? */
            else if (0xc0 == (ch & 0xc0))
            {
                *prim = VCJSON_LEXER_PRIM_LL_U8_2BYTE_START;
            }
            else if (0x80 == (ch & 0x80))
            {
                *prim = VCJSON_LEXER_PRIM_LL_U8_CONTINUATION;
            }
            else
            {
                *prim = VCJSON_LEXER_SYMBOL_INTERNAL_ERROR;
            }
            break;
    }

    /* if we did not fail decoding... */
    if (VCJSON_LEXER_SYMBOL_INTERNAL_ERROR != *prim)
    {
        /* update offset and position if lookahead is false */
        if (!lookahead)
        {
            *position = *offset;
            *offset += 1;
        }
        /* success. */
        return STATUS_SUCCESS;
    }

    /* set error position. */
    *position = *offset;
    /* we failed. */
    return ERROR_VCJSON_SCAN;
}
