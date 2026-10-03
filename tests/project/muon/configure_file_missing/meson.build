# SPDX-FileCopyrightText: VaiTon <eyadlorenzo@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

project('configure_file_missing')

fs = import('fs')

conf_data = configuration_data()
conf_data.set('KNOWN', 'value')

configure_file(
    input: 'template.in',
    output: 'template.out',
    configuration: conf_data,
)

output = fs.read(meson.current_build_dir() / 'template.out')
assert(
    output == 'known value; unknown @DEFAULT_SINK@, @DEFAULT_SOURCE@ and @DEFAULT_MONITOR@\n',
)
