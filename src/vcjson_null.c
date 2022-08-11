/**
 * \file vcjson_null.c
 *
 * \brief The VCJSON_NULL singleton.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#include <vcjson/vcjson.h>

#include "vcjson_internal.h"

static vcjson_null VCJSON_NULL_IMPL = {
    .hdr = { .release = &vcjson_null_resource_release
} };

vcjson_null* VCJSON_NULL = &VCJSON_NULL_IMPL;
