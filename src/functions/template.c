/*
 * SPDX-FileCopyrightText: Stone Tickle <lattis@mochiro.moe>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "compat.h"

#include "lang/func_lookup.h"
#include "functions/xxx.h"

static bool
func_(struct workspace *wk, obj self, obj *res)
{
}

const struct func_impl impl_tbl_xxx[] = {
	{ "", func_ },
	{ NULL, NULL },
};
