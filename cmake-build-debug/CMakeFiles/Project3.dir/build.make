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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /classes/OS/connor/connor.3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /classes/OS/connor/connor.3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Project3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Project3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Project3.dir/flags.make

CMakeFiles/Project3.dir/oss.c.o: CMakeFiles/Project3.dir/flags.make
CMakeFiles/Project3.dir/oss.c.o: ../oss.c
	$(CMAKE_COMMAND) -E cmake_progress_report /classes/OS/connor/connor.3/cmake-build-debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/Project3.dir/oss.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Project3.dir/oss.c.o   -c /classes/OS/connor/connor.3/oss.c

CMakeFiles/Project3.dir/oss.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Project3.dir/oss.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /classes/OS/connor/connor.3/oss.c > CMakeFiles/Project3.dir/oss.c.i

CMakeFiles/Project3.dir/oss.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Project3.dir/oss.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /classes/OS/connor/connor.3/oss.c -o CMakeFiles/Project3.dir/oss.c.s

CMakeFiles/Project3.dir/oss.c.o.requires:
.PHONY : CMakeFiles/Project3.dir/oss.c.o.requires

CMakeFiles/Project3.dir/oss.c.o.provides: CMakeFiles/Project3.dir/oss.c.o.requires
	$(MAKE) -f CMakeFiles/Project3.dir/build.make CMakeFiles/Project3.dir/oss.c.o.provides.build
.PHONY : CMakeFiles/Project3.dir/oss.c.o.provides

CMakeFiles/Project3.dir/oss.c.o.provides.build: CMakeFiles/Project3.dir/oss.c.o

CMakeFiles/Project3.dir/oosrcv.c.o: CMakeFiles/Project3.dir/flags.make
CMakeFiles/Project3.dir/oosrcv.c.o: ../oosrcv.c
	$(CMAKE_COMMAND) -E cmake_progress_report /classes/OS/connor/connor.3/cmake-build-debug/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/Project3.dir/oosrcv.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Project3.dir/oosrcv.c.o   -c /classes/OS/connor/connor.3/oosrcv.c

CMakeFiles/Project3.dir/oosrcv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Project3.dir/oosrcv.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /classes/OS/connor/connor.3/oosrcv.c > CMakeFiles/Project3.dir/oosrcv.c.i

CMakeFiles/Project3.dir/oosrcv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Project3.dir/oosrcv.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /classes/OS/connor/connor.3/oosrcv.c -o CMakeFiles/Project3.dir/oosrcv.c.s

CMakeFiles/Project3.dir/oosrcv.c.o.requires:
.PHONY : CMakeFiles/Project3.dir/oosrcv.c.o.requires

CMakeFiles/Project3.dir/oosrcv.c.o.provides: CMakeFiles/Project3.dir/oosrcv.c.o.requires
	$(MAKE) -f CMakeFiles/Project3.dir/build.make CMakeFiles/Project3.dir/oosrcv.c.o.provides.build
.PHONY : CMakeFiles/Project3.dir/oosrcv.c.o.provides

CMakeFiles/Project3.dir/oosrcv.c.o.provides.build: CMakeFiles/Project3.dir/oosrcv.c.o

# Object files for target Project3
Project3_OBJECTS = \
"CMakeFiles/Project3.dir/oss.c.o" \
"CMakeFiles/Project3.dir/oosrcv.c.o"

# External object files for target Project3
Project3_EXTERNAL_OBJECTS =

Project3: CMakeFiles/Project3.dir/oss.c.o
Project3: CMakeFiles/Project3.dir/oosrcv.c.o
Project3: CMakeFiles/Project3.dir/build.make
Project3: CMakeFiles/Project3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable Project3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Project3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Project3.dir/build: Project3
.PHONY : CMakeFiles/Project3.dir/build

CMakeFiles/Project3.dir/requires: CMakeFiles/Project3.dir/oss.c.o.requires
CMakeFiles/Project3.dir/requires: CMakeFiles/Project3.dir/oosrcv.c.o.requires
.PHONY : CMakeFiles/Project3.dir/requires

CMakeFiles/Project3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Project3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Project3.dir/clean

CMakeFiles/Project3.dir/depend:
	cd /classes/OS/connor/connor.3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /classes/OS/connor/connor.3 /classes/OS/connor/connor.3 /classes/OS/connor/connor.3/cmake-build-debug /classes/OS/connor/connor.3/cmake-build-debug /classes/OS/connor/connor.3/cmake-build-debug/CMakeFiles/Project3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Project3.dir/depend

