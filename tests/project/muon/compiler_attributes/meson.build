# SPDX-License-Identifier: GPL-3.0-only

project('compiler attributes', 'c')

cc = meson.get_compiler('c')

possible_c_attributes = [
    'alloc_size',
    'fallthrough',
    'retain',
]

foreach attr : possible_c_attributes
    cc.has_function_attribute(attr)
endforeach

cc.compiles(
    '__attribute__((__no_reorder__)) int x;',
    args: '-Werror=attributes',
    name: '__attribute__((__no_reorder__))',
)

cc.compiles(
    '__asm__(".pushsection .note.test, \\"aR\\", %note\\n.popsection\\n");',
    name: 'assembler supports the "R" (SHF_GNU_RETAIN) section flag',
)
