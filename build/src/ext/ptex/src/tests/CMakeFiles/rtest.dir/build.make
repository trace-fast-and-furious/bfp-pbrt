# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.27.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.27.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build

# Include any dependencies generated for this target.
include src/ext/ptex/src/tests/CMakeFiles/rtest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/ext/ptex/src/tests/CMakeFiles/rtest.dir/compiler_depend.make

# Include the progress variables for this target.
include src/ext/ptex/src/tests/CMakeFiles/rtest.dir/progress.make

# Include the compile flags for this target's objects.
include src/ext/ptex/src/tests/CMakeFiles/rtest.dir/flags.make

src/ext/ptex/src/tests/CMakeFiles/rtest.dir/rtest.cpp.o: src/ext/ptex/src/tests/CMakeFiles/rtest.dir/flags.make
src/ext/ptex/src/tests/CMakeFiles/rtest.dir/rtest.cpp.o: /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/ptex/src/tests/rtest.cpp
src/ext/ptex/src/tests/CMakeFiles/rtest.dir/rtest.cpp.o: src/ext/ptex/src/tests/CMakeFiles/rtest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/ext/ptex/src/tests/CMakeFiles/rtest.dir/rtest.cpp.o"
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex/src/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/ext/ptex/src/tests/CMakeFiles/rtest.dir/rtest.cpp.o -MF CMakeFiles/rtest.dir/rtest.cpp.o.d -o CMakeFiles/rtest.dir/rtest.cpp.o -c /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/ptex/src/tests/rtest.cpp

src/ext/ptex/src/tests/CMakeFiles/rtest.dir/rtest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/rtest.dir/rtest.cpp.i"
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex/src/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/ptex/src/tests/rtest.cpp > CMakeFiles/rtest.dir/rtest.cpp.i

src/ext/ptex/src/tests/CMakeFiles/rtest.dir/rtest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/rtest.dir/rtest.cpp.s"
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex/src/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/ptex/src/tests/rtest.cpp -o CMakeFiles/rtest.dir/rtest.cpp.s

# Object files for target rtest
rtest_OBJECTS = \
"CMakeFiles/rtest.dir/rtest.cpp.o"

# External object files for target rtest
rtest_EXTERNAL_OBJECTS =

src/ext/ptex/src/tests/rtest: src/ext/ptex/src/tests/CMakeFiles/rtest.dir/rtest.cpp.o
src/ext/ptex/src/tests/rtest: src/ext/ptex/src/tests/CMakeFiles/rtest.dir/build.make
src/ext/ptex/src/tests/rtest: src/ext/ptex/src/ptex/libPtex.a
src/ext/ptex/src/tests/rtest: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/lib/libz.tbd
src/ext/ptex/src/tests/rtest: src/ext/ptex/src/tests/CMakeFiles/rtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable rtest"
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex/src/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ext/ptex/src/tests/CMakeFiles/rtest.dir/build: src/ext/ptex/src/tests/rtest
.PHONY : src/ext/ptex/src/tests/CMakeFiles/rtest.dir/build

src/ext/ptex/src/tests/CMakeFiles/rtest.dir/clean:
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex/src/tests && $(CMAKE_COMMAND) -P CMakeFiles/rtest.dir/cmake_clean.cmake
.PHONY : src/ext/ptex/src/tests/CMakeFiles/rtest.dir/clean

src/ext/ptex/src/tests/CMakeFiles/rtest.dir/depend:
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3 /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/ptex/src/tests /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex/src/tests /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex/src/tests/CMakeFiles/rtest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/ext/ptex/src/tests/CMakeFiles/rtest.dir/depend

