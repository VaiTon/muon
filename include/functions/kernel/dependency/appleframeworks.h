/*
 * SPDX-FileCopyrightText: Eyad Issa <eyadlorenzo@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef MUON_FUNCTIONS_KERNEL_DEPENDENCY_APPLEFRAMEWORKS_H
#define MUON_FUNCTIONS_KERNEL_DEPENDENCY_APPLEFRAMEWORKS_H

#include "lang/func_lookup.h"

struct dep_lookup_ctx;

bool get_dependency_appleframeworks(struct workspace *wk, struct dep_lookup_ctx *ctx, bool *found);

#endif
