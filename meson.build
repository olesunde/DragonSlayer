project('GUI Project', 'cpp',
  version : '0.1',
  default_options : ['warning_level=2',
                     'cpp_std=c++23',
                     'c_std=c17',
                     'default_library=static'])

animationwindow_dep = dependency('animationwindow', fallback: ['animationwindow', 'animationwindow_dep'])
std_lib_facilities_dep = dependency('std_lib_facilities', fallback: ['std_lib_facilities', 'std_lib_facilities_dep'])

compiler_flags = ['-Wconversion', '-fdiagnostics-color=always', '-Werror=return-type', '-static']

src = [
  'main.cpp',
  'Entities/Character.cpp',
  'Entities/Dragon.cpp',
  'Entities/Walruss.cpp',
  'Entities/Penguin.cpp',
  'Model/Game.cpp',
  'Model/Timer.cpp',
  'World/View/Camera.cpp',
  'World/World.cpp',
  'Animation/Animation.cpp',
  'Animation/Animator.cpp',
]

incdir = include_directories('.', 'Entities', 'Model', 'World/View', 'World', 'Animation')

if host_machine.system() == 'windows'
  ld_flags = ['-static', '-lstdc++exp']
else
  ld_flags = []
endif

exe = executable('program', src, include_directories : incdir, dependencies : [std_lib_facilities_dep, animationwindow_dep], cpp_args : compiler_flags, link_args: ld_flags)

# On Windows the program links SDL2 through import libraries, so the matching
# DLLs must sit next to program.exe or it will not start. Stage them as part of
# the build rather than relying on a manual copy step. The paths are passed as
# plain strings because meson's sandbox forbids grabbing subproject files.
if host_machine.system() == 'windows'
  fs = import('fs')
  copy_file = find_program('tools/copy_file.py')
  runtime_dlls = [
    ['SDL2.dll', 'subprojects/sdl2_windows/bin/SDL2.dll'],
    ['SDL2_image.dll', 'subprojects/sdl2_image_windows/bin/SDL2_image.dll'],
    ['SDL2_mixer.dll', 'subprojects/sdl2_mixer_windows/bin/SDL2_mixer.dll'],
  ]
  foreach dll : runtime_dlls
    src = join_paths(meson.current_source_dir(), dll[1])
    if fs.exists(src)
      custom_target(dll[0],
        output : dll[0],
        command : [copy_file, src, '@OUTPUT@'],
        build_by_default : true)
    endif
  endforeach
endif
