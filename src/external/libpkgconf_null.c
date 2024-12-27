/*
 * SPDX-FileCopyrightText: Stone Tickle <lattis@mochiro.moe>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "compat.h"

#include "external/libpkgconf.h"
#include "log.h"

const bool have_libpkgconf = false;

bool
muon_pkgconf_lookup(struct workspace *wk, obj compiler, obj name, bool is_static, struct pkgconf_info *info)
{
	LOG_W("libpkgconf not enabled");
	return false;
}

bool
muon_pkgconf_get_variable(struct workspace *wk, const char *pkg_name, const char *var, obj *res)
{
	LOG_W("libpkgconf not enabled");
	return false;
}

bool
muon_pkgconf_define(struct workspace *wk, const char *key, const char *value)
{
	LOG_W("libpkgconf not enabled");
	return false;
}
