# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/m/ws_orb2/src/VISO2+

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/m/ws_orb2/src/VISO2+/builld

# Include any dependencies generated for this target.
include src/CMakeFiles/libtest.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/libtest.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/libtest.dir/flags.make

src/CMakeFiles/libtest.dir/config.cpp.o: src/CMakeFiles/libtest.dir/flags.make
src/CMakeFiles/libtest.dir/config.cpp.o: ../src/config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/m/ws_orb2/src/VISO2+/builld/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/libtest.dir/config.cpp.o"
	cd /home/m/ws_orb2/src/VISO2+/builld/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libtest.dir/config.cpp.o -c /home/m/ws_orb2/src/VISO2+/src/config.cpp

src/CMakeFiles/libtest.dir/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libtest.dir/config.cpp.i"
	cd /home/m/ws_orb2/src/VISO2+/builld/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/m/ws_orb2/src/VISO2+/src/config.cpp > CMakeFiles/libtest.dir/config.cpp.i

src/CMakeFiles/libtest.dir/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libtest.dir/config.cpp.s"
	cd /home/m/ws_orb2/src/VISO2+/builld/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/m/ws_orb2/src/VISO2+/src/config.cpp -o CMakeFiles/libtest.dir/config.cpp.s

src/CMakeFiles/libtest.dir/config.cpp.o.requires:

.PHONY : src/CMakeFiles/libtest.dir/config.cpp.o.requires

src/CMakeFiles/libtest.dir/config.cpp.o.provides: src/CMakeFiles/libtest.dir/config.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/libtest.dir/build.make src/CMakeFiles/libtest.dir/config.cpp.o.provides.build
.PHONY : src/CMakeFiles/libtest.dir/config.cpp.o.provides

src/CMakeFiles/libtest.dir/config.cpp.o.provides.build: src/CMakeFiles/libtest.dir/config.cpp.o


src/CMakeFiles/libtest.dir/camera.cpp.o: src/CMakeFiles/libtest.dir/flags.make
src/CMakeFiles/libtest.dir/camera.cpp.o: ../src/camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/m/ws_orb2/src/VISO2+/builld/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/libtest.dir/camera.cpp.o"
	cd /home/m/ws_orb2/src/VISO2+/builld/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libtest.dir/camera.cpp.o -c /home/m/ws_orb2/src/VISO2+/src/camera.cpp

src/CMakeFiles/libtest.dir/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libtest.dir/camera.cpp.i"
	cd /home/m/ws_orb2/src/VISO2+/builld/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/m/ws_orb2/src/VISO2+/src/camera.cpp > CMakeFiles/libtest.dir/camera.cpp.i

src/CMakeFiles/libtest.dir/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libtest.dir/camera.cpp.s"
	cd /home/m/ws_orb2/src/VISO2+/builld/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/m/ws_orb2/src/VISO2+/src/camera.cpp -o CMakeFiles/libtest.dir/camera.cpp.s

src/CMakeFiles/libtest.dir/camera.cpp.o.requires:

.PHONY : src/CMakeFiles/libtest.dir/camera.cpp.o.requires

src/CMakeFiles/libtest.dir/camera.cpp.o.provides: src/CMakeFiles/libtest.dir/camera.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/libtest.dir/build.make src/CMakeFiles/libtest.dir/camera.cpp.o.provides.build
.PHONY : src/CMakeFiles/libtest.dir/camera.cpp.o.provides

src/CMakeFiles/libtest.dir/camera.cpp.o.provides.build: src/CMakeFiles/libtest.dir/camera.cpp.o


# Object files for target libtest
libtest_OBJECTS = \
"CMakeFiles/libtest.dir/config.cpp.o" \
"CMakeFiles/libtest.dir/camera.cpp.o"

# External object files for target libtest
libtest_EXTERNAL_OBJECTS =

../lib/liblibtest.a: src/CMakeFiles/libtest.dir/config.cpp.o
../lib/liblibtest.a: src/CMakeFiles/libtest.dir/camera.cpp.o
../lib/liblibtest.a: src/CMakeFiles/libtest.dir/build.make
../lib/liblibtest.a: src/CMakeFiles/libtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/m/ws_orb2/src/VISO2+/builld/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../lib/liblibtest.a"
	cd /home/m/ws_orb2/src/VISO2+/builld/src && $(CMAKE_COMMAND) -P CMakeFiles/libtest.dir/cmake_clean_target.cmake
	cd /home/m/ws_orb2/src/VISO2+/builld/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/libtest.dir/build: ../lib/liblibtest.a

.PHONY : src/CMakeFiles/libtest.dir/build

src/CMakeFiles/libtest.dir/requires: src/CMakeFiles/libtest.dir/config.cpp.o.requires
src/CMakeFiles/libtest.dir/requires: src/CMakeFiles/libtest.dir/camera.cpp.o.requires

.PHONY : src/CMakeFiles/libtest.dir/requires

src/CMakeFiles/libtest.dir/clean:
	cd /home/m/ws_orb2/src/VISO2+/builld/src && $(CMAKE_COMMAND) -P CMakeFiles/libtest.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/libtest.dir/clean

src/CMakeFiles/libtest.dir/depend:
	cd /home/m/ws_orb2/src/VISO2+/builld && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/m/ws_orb2/src/VISO2+ /home/m/ws_orb2/src/VISO2+/src /home/m/ws_orb2/src/VISO2+/builld /home/m/ws_orb2/src/VISO2+/builld/src /home/m/ws_orb2/src/VISO2+/builld/src/CMakeFiles/libtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/libtest.dir/depend

