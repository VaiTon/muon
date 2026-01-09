/*
 * SPDX-FileCopyrightText: Eyad Issa <eyadlorenzo@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "compat.h"

#include "functions/kernel/dependency/known.h"

#include <string.h>

#include "functions/kernel/dependency/appleframeworks.h"
#include "functions/kernel/dependency/boost.h"
#include "functions/kernel/dependency/curses.h"
#include "functions/kernel/dependency/threads.h"

/*
 * Registry of known dependencies with custom lookup handlers
 *
 * This allows muon to handle dependencies like Meson does,
 * with specialized logic for libraries that need more than
 * just pkg-config detection.
 */

static const struct known_dependency known_dependencies[] = {
	{ "appleframeworks", get_dependency_appleframeworks },
	{ "boost", get_dependency_boost },
	{ "curses", get_dependency_curses },
	{ "threads", get_dependency_threads },
	{ NULL, NULL }, // sentinel
};

const struct known_dependency *
get_known_dependency(const struct str *name)
{
	for (uint32_t i = 0; known_dependencies[i].name; ++i) {
		if (str_eql(&STRL(known_dependencies[i].name), name) == 0) {
			return &known_dependencies[i];
		}
	}
	return NULL;
}
