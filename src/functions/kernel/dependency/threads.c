/*
 * SPDX-FileCopyrightText: Eyad Issa <eyadlorenzo@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "compat.h"

#include "functions/kernel/dependency.h"
#include "functions/kernel/dependency/threads.h"
#include "lang/object.h"
#include "log.h"

bool
get_dependency_threads(struct workspace *wk, struct dep_lookup_ctx *ctx, bool *found)
{
	*found = true;

	*ctx->res = make_obj(wk, obj_dependency);
	struct obj_dependency *dep = get_obj_dependency(wk, *ctx->res);
	dep->name = ctx->name;
	dep->flags |= dep_flag_found;
	dep->type = dependency_type_threads;

	dep->dep.compile_args = make_obj(wk, obj_array);
	obj_array_push(wk, dep->dep.compile_args, make_str(wk, "-pthread"));

	dep->dep.link_args = make_obj(wk, obj_array);
	obj_array_push(wk, dep->dep.link_args, make_str(wk, "-pthread"));

	L("threads dependency found");
	return true;
}
