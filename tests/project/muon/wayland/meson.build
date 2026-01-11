# SPDX-FileCopyrightText: Stone Tickle <lattis@mochiro.moe>
# SPDX-License-Identifier: GPL-3.0-only

project('test wayland meson module', 'c')

# Import the wayland Meson module
wl = import('wayland')

# Optionally, if the environment has wayland-scanner and wayland-client,
# you could exercise scan_xml and compile generated code. However, since this
# test should not hard fail on environments without Wayland, we limit ourselves
# to verifying the module’s API presence. The full integration is covered by
# projects that depend on Wayland.
#
# Example (not executed here due to environment variability):
wl_dep = dependency('wayland-client', required: false)
wl_scanner = dependency('wayland-scanner', required: false, native: true)
if wl_dep.found() and wl_scanner.found()
    xml = wl.find_protocol('xdg-shell', state: 'stable')
    gen = wl.scan_xml(xml)
    # Include a minimal hand-written main so the generated protocol code can link.
    # This uses the provided `wayland_demo.c` in the same directory.
    executable('wayland_demo', gen + ['wayland_demo.c'], dependencies: wl_dep)
endif
