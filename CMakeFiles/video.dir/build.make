# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ylf/work-place/videoaddresspart

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ylf/work-place/videoaddresspart

# Include any dependencies generated for this target.
include CMakeFiles/video.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/video.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/video.dir/flags.make

CMakeFiles/video.dir/test_lib.cpp.o: CMakeFiles/video.dir/flags.make
CMakeFiles/video.dir/test_lib.cpp.o: test_lib.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ylf/work-place/videoaddresspart/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/video.dir/test_lib.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/video.dir/test_lib.cpp.o -c /home/ylf/work-place/videoaddresspart/test_lib.cpp

CMakeFiles/video.dir/test_lib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/video.dir/test_lib.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ylf/work-place/videoaddresspart/test_lib.cpp > CMakeFiles/video.dir/test_lib.cpp.i

CMakeFiles/video.dir/test_lib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/video.dir/test_lib.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ylf/work-place/videoaddresspart/test_lib.cpp -o CMakeFiles/video.dir/test_lib.cpp.s

CMakeFiles/video.dir/test_lib.cpp.o.requires:
.PHONY : CMakeFiles/video.dir/test_lib.cpp.o.requires

CMakeFiles/video.dir/test_lib.cpp.o.provides: CMakeFiles/video.dir/test_lib.cpp.o.requires
	$(MAKE) -f CMakeFiles/video.dir/build.make CMakeFiles/video.dir/test_lib.cpp.o.provides.build
.PHONY : CMakeFiles/video.dir/test_lib.cpp.o.provides

CMakeFiles/video.dir/test_lib.cpp.o.provides.build: CMakeFiles/video.dir/test_lib.cpp.o

# Object files for target video
video_OBJECTS = \
"CMakeFiles/video.dir/test_lib.cpp.o"

# External object files for target video
video_EXTERNAL_OBJECTS =

src/video: CMakeFiles/video.dir/test_lib.cpp.o
src/video: CMakeFiles/video.dir/build.make
src/video: src/libvideoaddress.a
src/video: src/json/libjson.a
src/video: CMakeFiles/video.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable src/video"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/video.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/video.dir/build: src/video
.PHONY : CMakeFiles/video.dir/build

CMakeFiles/video.dir/requires: CMakeFiles/video.dir/test_lib.cpp.o.requires
.PHONY : CMakeFiles/video.dir/requires

CMakeFiles/video.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/video.dir/cmake_clean.cmake
.PHONY : CMakeFiles/video.dir/clean

CMakeFiles/video.dir/depend:
	cd /home/ylf/work-place/videoaddresspart && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ylf/work-place/videoaddresspart /home/ylf/work-place/videoaddresspart /home/ylf/work-place/videoaddresspart /home/ylf/work-place/videoaddresspart /home/ylf/work-place/videoaddresspart/CMakeFiles/video.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/video.dir/depend

