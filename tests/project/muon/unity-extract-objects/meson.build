project('unity-extract-objects', 'c')

lib = static_library('lib', 'one.c', 'two.c')
executable('consumer', 'main.c', objects: lib.extract_objects('one.c'))
