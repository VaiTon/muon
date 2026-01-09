/*
 * SPDX-FileCopyrightText: Eyad Issa <eyadlorenzo@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "compat.h"

#include "functions/kernel/dependency/curses.h"

#include "functions/kernel/dependency.h"
#include "lang/object.h"
#include "log.h"

bool
get_dependency_curses(struct workspace *wk, struct dep_lookup_ctx *ctx, bool *found)
{
	// Curses is an alias to ncurses
	L("curses dependency requested, trying ncurses");
	ctx->name = make_str(wk, "ncurses");
	// Fallback to normal dependency lookup
	*found = false;
	return true;
}
