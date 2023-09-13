# CMake generated Testfile for 
# Source directory: /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3
# Build directory: /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pbrt_unit_test "pbrt_test")
set_tests_properties(pbrt_unit_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/CMakeLists.txt;568;ADD_TEST;/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/CMakeLists.txt;0;")
subdirs("src/ext/openexr")
subdirs("src/ext/glog")
subdirs("src/ext/ptex")
