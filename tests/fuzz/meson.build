# SPDX-FileCopyrightText: Stone Tickle <lattis@mochiro.moe>
# SPDX-License-Identifier: GPL-3.0-only

afl_fuzz = find_program('afl-fuzz', required: false)

if not (cc.cmd_array()[0].startswith('afl-clang') and afl_fuzz.found())
    subdir_done()
endif

fuzz_c_args = muon_c_args + cc.get_supported_arguments('-Wno-gnu-statement-expression-from-macro-expansion')

fuzz_harness = executable(
    'fuzz-harness',
    'harness.c',
    include_directories: include_dir,
    dependencies: deps,
    link_with: muon_lib,
    c_args: fuzz_c_args,
    cpp_args: fuzz_c_args,
)

run_target(
    'fuzz-lang',
    command: [
        afl_fuzz,
        '-x', meson.current_source_dir() / 'meson.dict',
        '-i', meson.current_source_dir() / 'lang',
        '-o', meson.current_build_dir() / 'lang',
        '--', fuzz_harness,
        '@@',
    ],
)

# Fuzzing serial_load has a low signal to noise ratio, since serial_load
# doesn't make any guarantees about de-serializing untrusted input (much like
# python's pickle).  However, some of the crashes that afl found have been
# useful, so I'm including the run target here anyway.

subdir('dump-input')

run_target(
    'fuzz-dump',
    command: [
        afl_fuzz,
        '-i', fuzz_dump_input,
        '-o', meson.current_build_dir() / 'dump',
        '--', fuzz_harness,
        meson.current_source_dir() / 'load_dump.meson',
        '@@',
    ],
    depends: [fuzz_dump_input_dat],
)
