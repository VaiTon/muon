/*
 * Minimal wayland demo program used by tests.
 * Provides a definition for main so the test executable can link.
 *
 * This version exercises linking against libwayland-client by calling
 * a small subset of the API: `wl_display_connect` and `wl_display_disconnect`.
 *
 * The test meson.build arranges for this source to be compiled only when
 * the Wayland client dependency is found, so including the Wayland header
 * and calling its functions will exercise the linker.
 */

#include <stdio.h>
#include <wayland-client.h>

int main(void)
{
    struct wl_display *display = wl_display_connect(NULL);
    if (display) {
        puts("wayland_demo: wl_display_connect() succeeded");
        wl_display_disconnect(display);
    } else {
        puts("wayland_demo: wl_display_connect() returned NULL (no compositor?)");
    }
    return 0;
}
