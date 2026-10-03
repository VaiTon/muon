# SPDX-FileCopyrightText: VaiTon <eyadlorenzo@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

project('gnome_generate_gir_custom_include', 'c')

gnome = import('gnome')

gir_include = custom_target(
    'gir_include',
    input: 'include.gir.in',
    output: 'Include-1.0.gir',
    command: [find_program('cp'), '@INPUT@', '@OUTPUT@'],
)

libexample = shared_library('example', 'example.c', install: false)

gnome.generate_gir(
    libexample,
    includes: gir_include,
    header: 'example.h',
    namespace: 'Example',
    nsversion: '1.0',
    sources: 'example.c',
    install: false,
)
