/**
 * \file vcjson_bool.c
 *
 * \brief The VCJSON_BOOL singleton.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

vcjson_bool VCJSON_BOOL_TRUE_IMPL = {
    .hdr = { .release = &vcjson_bool_resource_release },
    .value = true
};

vcjson_bool VCJSON_BOOL_FALSE_IMPL = {
    .hdr = { .release = &vcjson_bool_resource_release },
    .value = false
};

const vcjson_bool* VCJSON_TRUE = &VCJSON_BOOL_TRUE_IMPL;
const vcjson_bool* VCJSON_FALSE = &VCJSON_BOOL_FALSE_IMPL;
