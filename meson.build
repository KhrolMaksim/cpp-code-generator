project('cpp-code-generator', 'cpp',
  version : '0.2',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

includes = include_directories('include')

sources = ['src/CodeStructure.cpp']

# Создаем статическую библиотеку
lib = static_library('cpp-code-generator',
  sources,
  include_directories : includes,
  install : true)

# Создаем pkg-config файл
pkg = import('pkgconfig')
pkg.generate(lib,
  name : 'cpp-code-generator',
  description : 'C++ Code Generator Library',
  version : meson.project_version(),
  requires : [],
  subdirs : ['cpp-code-generator'])

# Создаем исполняемый файл для тестов
executable('cpp-code-generator-test',
  'main.cpp',
  include_directories : includes,
  link_with : lib,
  install : false)
