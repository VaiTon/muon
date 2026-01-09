/*
 * SPDX-FileCopyrightText: Eyad Issa <eyadlorenzo@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "compat.h"

#include "functions/kernel/dependency/boost.h"

#include <stdlib.h>
#include <string.h>

#include "buf_size.h"
#include "error.h"
#include "functions/kernel/dependency.h"
#include "lang/object.h"
#include "lang/object_iterators.h"
#include "log.h"
#include "platform/filesystem.h"
#include "platform/path.h"

/*
 * Boost dependency detection
 *
 * This implementation is inspired by Meson's boost.py
 * It supports finding boost headers and libraries across various platforms.
 */

// Boost library compile args for different link types
struct boost_library_args {
	const char *name;
	const char *shared_args[4];
	const char *static_args[4];
	const char *multi_args[4];
	const char *single_args[4];
};

// List of known boost libraries with their compile flags
static const struct boost_library_args boost_libraries[] = {
	{
		.name = "boost_atomic",
		.shared_args = { "-DBOOST_ATOMIC_DYN_LINK=1", NULL },
		.static_args = { "-DBOOST_ATOMIC_STATIC_LINK=1", NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_chrono",
		.shared_args = { "-DBOOST_CHRONO_DYN_LINK=1", NULL },
		.static_args = { "-DBOOST_CHRONO_STATIC_LINK=1", NULL },
		.multi_args = { NULL },
		.single_args = { "-DBOOST_CHRONO_THREAD_DISABLED", NULL },
	},
	{
		.name = "boost_container",
		.shared_args = { "-DBOOST_CONTAINER_DYN_LINK=1", NULL },
		.static_args = { "-DBOOST_CONTAINER_STATIC_LINK=1", NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_context",
		.shared_args = { "-DBOOST_CONTEXT_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_contract",
		.shared_args = { "-DBOOST_CONTRACT_DYN_LINK", NULL },
		.static_args = { "-DBOOST_CONTRACT_STATIC_LINK", NULL },
		.multi_args = { NULL },
		.single_args = { "-DBOOST_CONTRACT_DISABLE_THREADS", NULL },
	},
	{
		.name = "boost_coroutine",
		.shared_args = { "-DBOOST_COROUTINES_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_date_time",
		.shared_args = { "-DBOOST_DATE_TIME_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_fiber",
		.shared_args = { "-DBOOST_FIBERS_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_filesystem",
		.shared_args = { "-DBOOST_FILESYSTEM_DYN_LINK=1", NULL },
		.static_args = { "-DBOOST_FILESYSTEM_STATIC_LINK=1", NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_iostreams",
		.shared_args = { "-DBOOST_IOSTREAMS_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_log",
		.shared_args = { "-DBOOST_LOG_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { "-DBOOST_LOG_NO_THREADS", NULL },
	},
	{
		.name = "boost_log_setup",
		.shared_args = { "-DBOOST_LOG_SETUP_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { "-DBOOST_LOG_NO_THREADS", NULL },
	},
	{
		.name = "boost_nowide",
		.shared_args = { "-DBOOST_NOWIDE_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_prg_exec_monitor",
		.shared_args = { "-DBOOST_TEST_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_random",
		.shared_args = { "-DBOOST_RANDOM_DYN_LINK", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_system",
		.shared_args = { "-DBOOST_SYSTEM_DYN_LINK=1", NULL },
		.static_args = { "-DBOOST_SYSTEM_STATIC_LINK=1", NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_test_exec_monitor",
		.shared_args = { "-DBOOST_TEST_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_thread",
		.shared_args = { "-DBOOST_THREAD_BUILD_DLL=1", "-DBOOST_THREAD_USE_DLL=1", NULL },
		.static_args = { "-DBOOST_THREAD_BUILD_LIB=1", "-DBOOST_THREAD_USE_LIB=1", NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_timer",
		.shared_args = { "-DBOOST_TIMER_DYN_LINK=1", NULL },
		.static_args = { "-DBOOST_TIMER_STATIC_LINK=1", NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_type_erasure",
		.shared_args = { "-DBOOST_TYPE_ERASURE_DYN_LINK", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{
		.name = "boost_unit_test_framework",
		.shared_args = { "-DBOOST_TEST_DYN_LINK=1", NULL },
		.static_args = { NULL },
		.multi_args = { NULL },
		.single_args = { NULL },
	},
	{ .name = NULL }, // sentinel
};

static const struct boost_library_args *
find_boost_library_args(const char *name)
{
	for (uint32_t i = 0; boost_libraries[i].name; ++i) {
		if (strcmp(boost_libraries[i].name, name) == 0) {
			return &boost_libraries[i];
		}
	}
	return NULL;
}

static bool
find_boost_include_dir(struct workspace *wk, const char *root, obj *include_dir, obj *version)
{
	TSTR(root_path);
	tstr_pushs(wk, &root_path, root);

	// Try multiple locations for boost headers
	const char *search_paths[] = {
		"include/boost",
		"boost",
		NULL,
	};

	for (uint32_t i = 0; search_paths[i]; ++i) {
		TSTR(check_path);
		path_join(wk, &check_path, root_path.buf, search_paths[i]);

		TSTR(version_header);
		path_join(wk, &version_header, check_path.buf, "version.hpp");

		if (fs_file_exists(version_header.buf)) {
			// Found boost headers, try to extract version
			*include_dir = make_str(wk, check_path.buf);

			// For now, just mark version as found
			// TODO: Parse version.hpp to extract actual version
			*version = make_str(wk, "unknown");

			L("found boost headers in %s", check_path.buf);
			return true;
		}
	}

	return false;
}

struct find_boost_libs_ctx {
	const char *lib_dir;
	obj libs;
};

static enum iteration_result
find_boost_lib_iter(struct workspace *wk, void *_ctx, obj module)
{
	struct find_boost_libs_ctx *ctx = _ctx;
	const struct str *mod_str = get_str(wk, module);

	// Construct library name with boost_ prefix
	TSTR(lib_name);
	tstr_pushs(wk, &lib_name, "boost_");
	tstr_pushs(wk, &lib_name, mod_str->s);

	// Try to find the library file
	// On Unix: libboost_<module>.so or libboost_<module>.a
	// On Windows: boost_<module>.lib or boost_<module>.dll
	TSTR(lib_path);
	path_join(wk, &lib_path, ctx->lib_dir, "lib");
	tstr_pushs(wk, &lib_path, lib_name.buf);
	tstr_pushs(wk, &lib_path, ".so");

	if (fs_file_exists(lib_path.buf)) {
		obj_array_push(wk, ctx->libs, tstr_into_str(wk, &lib_path));
		L("found boost library %s", lib_path.buf);
	} else {
		// Try .a extension
		lib_path.len = 0;
		path_join(wk, &lib_path, ctx->lib_dir, "lib");
		tstr_pushs(wk, &lib_path, lib_name.buf);
		tstr_pushs(wk, &lib_path, ".a");

		if (fs_file_exists(lib_path.buf)) {
			obj_array_push(wk, ctx->libs, tstr_into_str(wk, &lib_path));
			L("found boost library %s", lib_path.buf);
		}
	}

	return ir_cont;
}

static bool
find_boost_libraries(struct workspace *wk, const char *root, obj modules, obj *libs)
{
	TSTR(lib_dir);
	tstr_pushs(wk, &lib_dir, root);
	path_push(wk, &lib_dir, "lib");

	if (!fs_dir_exists(lib_dir.buf)) {
		return false;
	}

	*libs = make_obj(wk, obj_array);

	struct find_boost_libs_ctx ctx = {
		.lib_dir = lib_dir.buf,
		.libs = *libs,
	};

	obj_array_foreach(wk, modules, &ctx, find_boost_lib_iter);

	return true;
}

struct add_boost_lib_args_ctx {
	obj compile_args;
};

static enum iteration_result
add_boost_lib_args_iter(struct workspace *wk, void *_ctx, obj module)
{
	struct add_boost_lib_args_ctx *ctx = _ctx;

	TSTR(lib_name);
	tstr_pushs(wk, &lib_name, "boost_");
	tstr_pushs(wk, &lib_name, get_cstr(wk, module));

	const struct boost_library_args *lib_args = find_boost_library_args(lib_name.buf);
	if (lib_args) {
		// Add shared/static specific args
		// For now, assume shared libraries
		for (uint32_t i = 0; lib_args->shared_args[i]; ++i) {
			obj_array_push(wk, ctx->compile_args, make_str(wk, lib_args->shared_args[i]));
		}
	}

	return ir_cont;
}

static enum iteration_result
add_lib_to_link_args_iter(struct workspace *wk, void *_ctx, obj lib)
{
	obj *link_args = _ctx;
	obj_array_push(wk, *link_args, lib);
	return ir_cont;
}

bool
get_dependency_boost(struct workspace *wk, struct dep_lookup_ctx *ctx, bool *found)
{
	*found = false;

	// Get modules list (requested boost libraries)
	obj modules = ctx->modules;

	if (!modules) {
		// No modules requested, just provide headers
		modules = make_obj(wk, obj_array);
	} else if (get_obj_type(wk, modules) != obj_array) {
		vm_error_at(wk, ctx->err_node, "boost modules must be an array");
		return false;
	}

	// Try to find boost root from environment or common locations
	const char *boost_root = getenv("BOOST_ROOT");

	if (!boost_root) {
		// Try common system locations
		const char *common_roots[] = {
			"/usr",
			"/usr/local",
			"/opt/local",
			NULL,
		};

		for (uint32_t i = 0; common_roots[i]; ++i) {
			obj include_dir, version;
			if (find_boost_include_dir(wk, common_roots[i], &include_dir, &version)) {
				boost_root = common_roots[i];
				break;
			}
		}
	}

	if (!boost_root) {
		L("boost not found");
		return true;
	}

	obj include_dir, version;
	if (!find_boost_include_dir(wk, boost_root, &include_dir, &version)) {
		L("boost headers not found in %s", boost_root);
		return true;
	}

	// Create dependency object
	*ctx->res = make_obj(wk, obj_dependency);
	struct obj_dependency *dep = get_obj_dependency(wk, *ctx->res);
	dep->name = ctx->name;
	dep->version = version;
	dep->flags |= dep_flag_found;
	dep->type = dependency_type_system;
	dep->machine = ctx->machine;

	// Set up compile args
	dep->dep.compile_args = make_obj(wk, obj_array);

	// Add include directory
	TSTR(inc_arg);
	tstr_pushs(wk, &inc_arg, "-I");
	tstr_pushs(wk, &inc_arg, get_cstr(wk, include_dir));
	obj_array_push(wk, dep->dep.compile_args, tstr_into_str(wk, &inc_arg));

	// Add BOOST_ALL_NO_LIB define to disable automatic linking
	obj_array_push(wk, dep->dep.compile_args, make_str(wk, "-DBOOST_ALL_NO_LIB"));

	// Set up link args if modules are requested
	dep->dep.link_args = make_obj(wk, obj_array);

	if (get_obj_array(wk, modules)->len > 0) {
		obj libs;
		if (find_boost_libraries(wk, boost_root, modules, &libs)) {
			// Add library compile args for each module
			struct add_boost_lib_args_ctx args_ctx = {
				.compile_args = dep->dep.compile_args,
			};
			obj_array_foreach(wk, modules, &args_ctx, add_boost_lib_args_iter);

			// Add link args for found libraries
			obj_array_foreach(wk, libs, &dep->dep.link_args, add_lib_to_link_args_iter);
		}
	}

	*found = true;
	L("boost dependency found at %s", boost_root);
	return true;
}
