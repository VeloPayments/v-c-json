project('vcjson', 'c', 'cpp',
  version : '0.4.3-snapshot',
  default_options : ['c_std=gnu11', 'cpp_std=c++14', 'buildtype=release'],
  meson_version : '>=0.53.0'
)

message('Using build type: ' + get_option('buildtype'))

force_velo_toolchain = get_option('force_velo_toolchain')

if force_velo_toolchain
  cc = meson.get_compiler('c')
  cc_version = run_command(cc.cmd_array() + ['--version'], check : true).stdout()
  if not cc_version.contains('(velo)')
    error('force_velo_toolchain is set and you aren\'t using the velo toolchain.')
  endif
  cpp = meson.get_compiler('cpp')
  cpp_version = run_command(cpp.cmd_array() + ['--version'], check : true).stdout()
  if not cpp_version.contains('(velo)')
    error('force_velo_toolchain is set and you aren\'t using the velo toolchain.')
  endif
else
  warning('Not using the blessed velo gcc toolchain. Here be dragons.')
endif

add_project_arguments('-Wall', '-Werror', '-Wextra', language : 'c')
add_project_arguments('-Wall', '-Werror', '-Wextra', language : 'cpp')

src = run_command('find', './src', '-name', '*.c', check : true).stdout().strip().split('\n')
test_src = run_command('find', './test', '-name', '*.cpp', check : true).stdout().strip().split('\n')

vcmodel = dependency('vcmodel',
  required : true,
  fallback : ['vcmodel', 'vcmodel_dep']
)

vcmodel_sub = subproject('vcmodel')
vcmodel_include = vcmodel_sub.get_variable('vcmodel_include')

cmake = import('cmake')

minunit_proj = cmake.subproject('minunit')
minunit = minunit_proj.dependency('minunit')

rcpr_proj = cmake.subproject('rcpr')
rcpr = rcpr_proj.dependency('rcpr')
rcpr_lib = rcpr_proj.target('rcpr')
rcpr_test = rcpr_proj.target('testrcpr')
rcpr_test_dep = rcpr_proj.target('test')
rcpr_include = rcpr_proj.include_directories('rcpr')

vcjson_lib_deps = [rcpr, vcmodel]

vcjson_include = include_directories('include')
config_include = include_directories('.')

vcjson_include_directories = [
  rcpr_include,
  vcmodel_include,
  vcjson_include,
]

vcjson_lib = static_library('vcjson', src,
  dependencies : vcjson_lib_deps,
  include_directories: [vcjson_include_directories, config_include],
)

vcjson_dep = declare_dependency(
  link_with : [vcjson_lib, rcpr_lib],
  include_directories : vcjson_include_directories
)

vcjson_test = executable('testvcjson', test_src,
  dependencies : [minunit, rcpr],
  include_directories: [vcjson_include_directories, config_include],
  link_with : vcjson_lib
)

test_env = environment()

test(
    'testvcjson',
    vcjson_test,
    env : test_env,
)

test('testrcpr', rcpr_test, depends: rcpr_test_dep)

conf_data = configuration_data()
conf_data.set('VERSION', meson.project_version())
configure_file(
  input : 'config.h.in',
  output : 'config.h',
  configuration : conf_data
)
