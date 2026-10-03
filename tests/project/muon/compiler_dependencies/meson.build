# SPDX-License-Identifier: GPL-3.0-only

project('compiler dependencies', 'c')

cc = meson.get_compiler('c')

add_project_arguments('-DMUON_TEST_PROJECT_C_ARG=1', language: 'c')
assert(
    cc.compiles(
        '#ifndef MUON_TEST_PROJECT_C_ARG\n#error missing project argument\n#endif',
        name: 'compiler checks use project arguments',
    ),
)

libm = cc.find_library('m', required: false)

if libm.found()
    assert(
        cc.has_function('cos', prefix: '#include <math.h>', dependencies: libm),
        'cc.has_function should link dependencies',
    )
endif
