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

# Utility rule file for ExperimentalStart.

# Include any custom commands dependencies for this target.
include src/ext/ptex/CMakeFiles/ExperimentalStart.dir/compiler_depend.make

# Include the progress variables for this target.
include src/ext/ptex/CMakeFiles/ExperimentalStart.dir/progress.make

src/ext/ptex/CMakeFiles/ExperimentalStart:
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex && /usr/local/Cellar/cmake/3.27.1/bin/ctest -D ExperimentalStart

ExperimentalStart: src/ext/ptex/CMakeFiles/ExperimentalStart
ExperimentalStart: src/ext/ptex/CMakeFiles/ExperimentalStart.dir/build.make
.PHONY : ExperimentalStart

# Rule to build all files generated by this target.
src/ext/ptex/CMakeFiles/ExperimentalStart.dir/build: ExperimentalStart
.PHONY : src/ext/ptex/CMakeFiles/ExperimentalStart.dir/build

src/ext/ptex/CMakeFiles/ExperimentalStart.dir/clean:
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex && $(CMAKE_COMMAND) -P CMakeFiles/ExperimentalStart.dir/cmake_clean.cmake
.PHONY : src/ext/ptex/CMakeFiles/ExperimentalStart.dir/clean

src/ext/ptex/CMakeFiles/ExperimentalStart.dir/depend:
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3 /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/ptex /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/ptex/CMakeFiles/ExperimentalStart.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/ext/ptex/CMakeFiles/ExperimentalStart.dir/depend

