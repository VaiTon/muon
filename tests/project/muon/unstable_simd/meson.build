# SPDX-FileCopyrightText: VaiTon <eyadlorenzo@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

project('unstable_simd', 'c')

simd = import('unstable-simd')
cc = meson.get_compiler('c')

if cc.get_supported_arguments(['-mmmx']).length() == 1
    result = simd.check(
        'simd_test',
        kwargs: {'mmx': ['mmx.c']},
        compiler: cc,
    )
    assert(result[0].length() == 1)
endif
