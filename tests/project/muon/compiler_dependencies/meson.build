# SPDX-License-Identifier: GPL-3.0-only

project('compiler dependencies', 'c')

cc = meson.get_compiler('c')
libm = cc.find_library('m', required: false)

if libm.found()
    assert(
        cc.has_function('cos', prefix: '#include <math.h>', dependencies: libm),
        'cc.has_function should link dependencies',
    )
endif
