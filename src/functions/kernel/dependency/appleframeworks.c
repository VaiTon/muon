/*
 * SPDX-FileCopyrightText: Eyad Issa <eyadlorenzo@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "compat.h"

#include "functions/kernel/dependency/appleframeworks.h"

#include "functions/kernel/dependency.h"

bool
get_dependency_appleframeworks(struct workspace *wk, struct dep_lookup_ctx *ctx, bool *found)
{
	// "appleframeworks" is just an alias that delegates to the generic extraframework handler
	return get_dependency_extraframework(wk, ctx, found);
}
