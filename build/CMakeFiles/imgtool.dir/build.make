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
include CMakeFiles/imgtool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/imgtool.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/imgtool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imgtool.dir/flags.make

CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o: CMakeFiles/imgtool.dir/flags.make
CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o: /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/tools/imgtool.cpp
CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o: CMakeFiles/imgtool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o -MF CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o.d -o CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o -c /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/tools/imgtool.cpp

CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/tools/imgtool.cpp > CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.i

CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/tools/imgtool.cpp -o CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.s

# Object files for target imgtool
imgtool_OBJECTS = \
"CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o"

# External object files for target imgtool
imgtool_EXTERNAL_OBJECTS =

imgtool: CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o
imgtool: CMakeFiles/imgtool.dir/build.make
imgtool: libpbrt.a
imgtool: src/ext/openexr/OpenEXR/IlmImf/libIlmImf.a
imgtool: src/ext/openexr/IlmBase/Imath/libImath.a
imgtool: src/ext/openexr/IlmBase/Half/libHalf.a
imgtool: src/ext/glog/libglog.a
imgtool: src/ext/ptex/src/ptex/libPtex.a
imgtool: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/lib/libz.tbd
imgtool: src/ext/openexr/IlmBase/IlmThread/libIlmThread.a
imgtool: src/ext/openexr/IlmBase/Iex/libIex.a
imgtool: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/lib/libz.tbd
imgtool: CMakeFiles/imgtool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable imgtool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imgtool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imgtool.dir/build: imgtool
.PHONY : CMakeFiles/imgtool.dir/build

CMakeFiles/imgtool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/imgtool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/imgtool.dir/clean

CMakeFiles/imgtool.dir/depend:
	cd /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3 /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3 /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/CMakeFiles/imgtool.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/imgtool.dir/depend
