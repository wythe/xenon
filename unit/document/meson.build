spec_exe = executable('spec', 'specunit.cpp', 'fieldtypes.cpp')
# target_link_libraries(['spec', 'xenon'])
test('NAME', 'spec', 'WORKING_DIRECTORY', cmake_current_source_dir, 'COMMAND', 'spec')
Boost_dep = dependency('Boost')
if 'Boost_FOUND'
  # include_directories(boost_include_dirs)
  # message('adding load test')
  load_exe = executable('load', 'loadunit.cpp')
  # target_link_libraries(['load', 'xenon'])
  # target_link_libraries(['load', boost_libraries])
  test('NAME', 'load', 'WORKING_DIRECTORY', cmake_current_source_dir, 'COMMAND', 'load')
endif
