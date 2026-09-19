#include "compat.h"
#include "lang/eval.h"
#include "lang/workspace.h"
#include "lang/typecheck.h"
#include "platform/init.h"
#include "platform/path.h"
#include "platform/filesystem.h"
#include "machines.h"
#include "log.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	platform_init();

	struct arena a;
	struct arena a_scratch;
	arena_init(&a, );
	arena_init(&a_scratch, );
	struct workspace wk;
	workspace_init_arena(&wk, &a, &a_scratch);

	// signal_handler is defined in src/main.c which we are NOT linking.
	// We can either define our own or just skip it for the fuzzer.
	// platform_set_signal_handler(signal_handler, &wk);

	log_set_file(&wk, stdout);
	log_set_lvl(log_error);

	path_init(&wk);
	machine_init();

#ifdef __AFL_HAVE_MANUAL_CONTROL
	__AFL_INIT();
#endif

#ifndef __AFL_COMPILER
#define __AFL_LOOP(x) 0
#endif

	while (__AFL_LOOP(10000)) {
		ar_clear(&a);
		ar_clear(&a_scratch);
		workspace_init_bare(&wk, &a, &a_scratch);

		wk.vm.disable_fuzz_unsafe_functions = true;

		struct source src = { 0 };

		if (argc < 2) {
			break;
		}

		if (!fs_read_entire_file(wk.a_scratch, argv[1], &src)) {
			continue;
		}

		struct args_norm an[] = { { .name = "argv" }, { ARG_TYPE_NULL } };
		an[0].val = make_obj(&wk, obj_array);
		for (int i = 2; i < argc; ++i) {
			obj_array_push(&wk, an[0].val, make_str(&wk, argv[i]));
		}

		obj res;
		eval(&wk, &src, &(struct eval_opts) { build_language_meson, language_internal, .an = an }, &res);
	}

#if !MUON_RELEASE
	ar_destroy(&a);
	ar_destroy(&a_scratch);
#endif

	return 0;
}