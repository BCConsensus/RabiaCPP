project('rabiacpp',
  'cpp',
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=c++20', 'buildtype=release']
)

incdir = include_directories('include')

cxx = meson.get_compiler('cpp')
boost_dep = [
    cxx.find_library('boost_system'),
    cxx.find_library('boost_filesystem'),
]

executable('rabiacpp',
           'main.cpp',
           install : true,
           include_directories : incdir,
           dependencies: boost_dep)
