project('chip8', 'c',
  version: 'pa0.1.0',
  default_options: ['warning_level=3', 'c_std=gnu2x', 'werror=true']
)

srcs = files(
  # files go here
  # (or in 'folder'/meson.build)
)

# optional subdir('folder')
subdir('src')

include = include_directories(
  'include'
)

sdl3 = dependency('SDL3')

executable('chip8', srcs,
  include_directories: include,
  install: true,
  dependencies: [sdl3]
)
