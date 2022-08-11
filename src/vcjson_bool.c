/**
 * \file vcjson_bool.c
 *
 * \brief The VCJSON_BOOL singleton.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

static vcjson_bool VCJSON_BOOL_TRUE_IMPL = {
    .hdr = { .release = &vcjson_bool_resource_release },
    .value = true
};

static vcjson_bool VCJSON_BOOL_FALSE_IMPL = {
    .hdr = { .release = &vcjson_bool_resource_release },
    .value = false
};

vcjson_bool* VCJSON_TRUE = &VCJSON_BOOL_TRUE_IMPL;
vcjson_bool* VCJSON_FALSE = &VCJSON_BOOL_FALSE_IMPL;
