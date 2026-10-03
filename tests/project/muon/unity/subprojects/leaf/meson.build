project('unity-leaf', 'c')

fs = import('fs')
unity_mode = get_option('unity')
assert(meson.is_unity() == (unity_mode == 'on' or unity_mode == 'subprojects'))

static_library('unity-leaf', 'leaf_one.c', 'leaf_two.c', 'leaf_three.c')
if meson.is_unity()
    assert(
        fs.is_file(meson.current_build_dir() / 'libunity-leaf.a.p/unity/unity_c_0.c'),
    )
else
    assert(
        not fs.is_file(meson.current_build_dir() / 'libunity-leaf.a.p/unity/unity_c_0.c'),
    )
endif
