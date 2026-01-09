/*
 * SPDX-FileCopyrightText: Stone Tickle <lattis@mochiro.moe>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef MUON_FUNCTIONS_KERNEL_DEPENDENCY_KNOWN_H
#define MUON_FUNCTIONS_KERNEL_DEPENDENCY_KNOWN_H

#include "lang/func_lookup.h"

struct dep_lookup_ctx;

typedef bool (*known_dependency_handler)(struct workspace *wk, struct dep_lookup_ctx *ctx, bool *found);

struct known_dependency {
	const char *name;
	known_dependency_handler handler;
};

const struct known_dependency *get_known_dependency(const struct str *name);

#endif
