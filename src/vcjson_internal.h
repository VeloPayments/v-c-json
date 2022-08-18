/**
 * \file vcjson_internal.h
 *
 * \brief Internal header for vcjson.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

#include <rcpr/rbtree.h>
#include <rcpr/resource/protected.h>
#include <rcpr/status.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

struct vcjson_number
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    double value;
};

struct vcjson_string
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    char* value;
    size_t length;
};

struct vcjson_null
{
    RCPR_SYM(resource) hdr;
};

struct vcjson_bool
{
    RCPR_SYM(resource) hdr;
    bool value;
};

struct vcjson_value
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    int type;
    void* value;
};

struct vcjson_object
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(rbtree)* elements;
};

/**
 * \brief An element in a JSON object.
 */
typedef struct vcjson_object_element vcjson_object_element;

struct vcjson_object_element
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    char* key;
    vcjson_value* value;
};

struct vcjson_object_iterator
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(rbtree)* tree;
    RCPR_SYM(rbtree_node)* nil;
    RCPR_SYM(rbtree_node)* curr;
};

struct vcjson_array
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    vcjson_value** arr;
    size_t elems;
};

extern vcjson_null VCJSON_NULL_IMPL;
extern vcjson_bool VCJSON_BOOL_TRUE_IMPL;
extern vcjson_bool VCJSON_BOOL_FALSE_IMPL;

/* lexer character constants. */
#define VCJSON_LEXER_PRIM_COLON                                              1
#define VCJSON_LEXER_PRIM_COMMA                                              2
#define VCJSON_LEXER_PRIM_DIGIT_0                                            3
#define VCJSON_LEXER_PRIM_DIGIT_1                                            4
#define VCJSON_LEXER_PRIM_DIGIT_2                                            5
#define VCJSON_LEXER_PRIM_DIGIT_3                                            6
#define VCJSON_LEXER_PRIM_DIGIT_4                                            7
#define VCJSON_LEXER_PRIM_DIGIT_5                                            8
#define VCJSON_LEXER_PRIM_DIGIT_6                                            9
#define VCJSON_LEXER_PRIM_DIGIT_7                                           10
#define VCJSON_LEXER_PRIM_DIGIT_8                                           11
#define VCJSON_LEXER_PRIM_DIGIT_9                                           12
#define VCJSON_LEXER_PRIM_DOT                                               13
#define VCJSON_LEXER_PRIM_HEX_a                                             14
#define VCJSON_LEXER_PRIM_HEX_A                                             15
#define VCJSON_LEXER_PRIM_HEX_b                                             16
#define VCJSON_LEXER_PRIM_HEX_B                                             17
#define VCJSON_LEXER_PRIM_HEX_c                                             18
#define VCJSON_LEXER_PRIM_HEX_C                                             19
#define VCJSON_LEXER_PRIM_HEX_d                                             20
#define VCJSON_LEXER_PRIM_HEX_D                                             21
#define VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_e                                 22
#define VCJSON_LEXER_PRIM_HEX_OR_EXPONENT_E                                 23
#define VCJSON_LEXER_PRIM_HEX_f                                             24
#define VCJSON_LEXER_PRIM_HEX_F                                             25
#define VCJSON_LEXER_PRIM_LEFT_BRACE                                        26
#define VCJSON_LEXER_PRIM_LEFT_BRACKET                                      27
#define VCJSON_LEXER_PRIM_MINUS                                             28
#define VCJSON_LEXER_PRIM_PLUS                                              29
#define VCJSON_LEXER_PRIM_QUOTE                                             30
#define VCJSON_LEXER_PRIM_RIGHT_BRACE                                       31
#define VCJSON_LEXER_PRIM_RIGHT_BRACKET                                     32

/* mid-level primitives. */
#define VCJSON_LEXER_PRIM_LL_U8_7BIT                                      1000
#define VCJSON_LEXER_PRIM_LL_U8_2BYTE_START                               1001
#define VCJSON_LEXER_PRIM_LL_U8_3BYTE_START                               1002
#define VCJSON_LEXER_PRIM_LL_U8_4BYTE_START                               1003
#define VCJSON_LEXER_PRIM_LL_U8_CONTINUATION                              1004
#define VCJSON_LEXER_PRIM_LL_WHITESPACE                                   1005
#define VCJSON_LEXER_PRIM_LL_NON_WS_CONTROL                               1006
#define VCJSON_LEXER_PRIM_LL_WS_CONTROL                                   1007

/* special EOF symbol. */
#define VCJSON_LEXER_SYMBOL_SPECIAL_EOF                                     -1

/* internal error symbol. */
#define VCJSON_LEXER_SYMBOL_INTERNAL_ERROR                                  -2

/* lower level symbols. */
#define VCJSON_LEXER_SYMBOL_LL_CODEPOINT                                  2000
#define VCJSON_LEXER_SYMBOL_LL_ESCAPE                                     2001

/* higher level symbols. */
#define VCJSON_LEXER_SYMBOL_FALSE                                         3000
#define VCJSON_LEXER_SYMBOL_NULL                                          3001
#define VCJSON_LEXER_SYMBOL_NUMBER                                        3002
#define VCJSON_LEXER_SYMBOL_STRING                                        3003
#define VCJSON_LEXER_SYMBOL_TRUE                                          3004

/**
 * \brief Comparison function for an object elements tree.
 *
 * \param context       Unused.
 * \param lhs           The left-hand-side key to compare.
 * \param rhs           The right-hand-side key to compare.
 *
 * \returns an integer value representing the comparison result.
 *      - RCPR_COMPARE_LT if \p lhs &lt; \p rhs.
 *      - RCPR_COMPARE_EQ if \p lhs == \p rhs.
 *      - RCPR_COMPARE_GT if \p lhs &gt; \p rhs.
 */
RCPR_SYM(rcpr_comparison_result) vcjson_object_element_compare(
    void* context, const void* lhs, const void* rhs);

/**
 * \brief Given an object element resource, return the key for this resource.
 *
 * \param context       Unused.
 * \param r             The resource.
 *
 * \returns the key for this resource.
 */
const void* vcjson_object_element_key(
    void* context, const RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_number resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_number_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_string resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_string_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_null resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_null_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_bool resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_bool_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_value singleton resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_value_singleton_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_value with a resource.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_value_with_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_object.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_object_element.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_element_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_object_iterator.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_resource_release(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref vcjson_array.
 *
 * \param r             The resource to release.
 */
status FN_DECL_MUST_CHECK
vcjson_array_resource_release(RCPR_SYM(resource)* r);

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
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_scan_primitive(
    int* prim, size_t* position, const char* input, size_t size,
    size_t* offset);

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
    size_t size, size_t* offset);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
