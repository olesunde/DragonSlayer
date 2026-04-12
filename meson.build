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
  'View/Camera.cpp',
  'World/World.cpp',
  'Animation/Animation.cpp',
  'Animation/Animator.cpp',
]

incdir = include_directories('.', 'Entities', 'Model', 'View', 'World', 'Animation')

if host_machine.system() == 'windows'
  ld_flags = ['-static', '-lstdc++exp']
else
  ld_flags = []
endif

exe = executable('program', src, include_directories : incdir, dependencies : [std_lib_facilities_dep, animationwindow_dep], cpp_args : compiler_flags, link_args: ld_flags)
