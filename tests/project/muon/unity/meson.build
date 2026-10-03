project('unity', ['c', 'cpp'])

fs = import('fs')
unity_mode = get_option('unity')
main_unity = unity_mode == 'on'
assert(meson.is_unity() == main_unity)

executable(
    'unity-main',
    'main.c',
    'one.c',
    'two.c',
    'three.c',
    'four.c',
    'five.cpp',
    'six.cpp',
)

static_library(
    'unity-disabled',
    'one.c',
    'two.c',
    override_options: ['unity=off'],
)

main_build_dir = meson.current_build_dir()
if main_unity
    assert(fs.is_file(main_build_dir / 'unity-main.p/unity/unity_c_0.c'))
    assert(fs.is_file(main_build_dir / 'unity-main.p/unity/unity_cpp_0.cc'))
    assert(
        fs.is_file(main_build_dir / 'unity-main.p/unity/unity_c_1.c') == (get_option('unity_size') == 2),
    )
else
    assert(not fs.is_file(main_build_dir / 'unity-main.p/unity/unity_c_0.c'))
endif
assert(not fs.is_file(main_build_dir / 'libunity-disabled.a.p/unity/unity_c_0.c'))

subproject('leaf')
