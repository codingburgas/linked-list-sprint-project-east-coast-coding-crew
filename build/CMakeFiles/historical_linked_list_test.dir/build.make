# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/build

# Include any dependencies generated for this target.
include CMakeFiles/historical_linked_list_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/historical_linked_list_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/historical_linked_list_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/historical_linked_list_test.dir/flags.make

CMakeFiles/historical_linked_list_test.dir/codegen:
.PHONY : CMakeFiles/historical_linked_list_test.dir/codegen

CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o: CMakeFiles/historical_linked_list_test.dir/flags.make
CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o: /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/eccc/Core/tests/historical_linked_list_test.cpp
CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o: CMakeFiles/historical_linked_list_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o -MF CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o.d -o CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o -c /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/eccc/Core/tests/historical_linked_list_test.cpp

CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/eccc/Core/tests/historical_linked_list_test.cpp > CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.i

CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/eccc/Core/tests/historical_linked_list_test.cpp -o CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.s

CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o: CMakeFiles/historical_linked_list_test.dir/flags.make
CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o: /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/eccc/Core/src/historical_linked_list.cpp
CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o: CMakeFiles/historical_linked_list_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o -MF CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o.d -o CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o -c /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/eccc/Core/src/historical_linked_list.cpp

CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.i"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/eccc/Core/src/historical_linked_list.cpp > CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.i

CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.s"
	/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/eccc/Core/src/historical_linked_list.cpp -o CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.s

# Object files for target historical_linked_list_test
historical_linked_list_test_OBJECTS = \
"CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o" \
"CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o"

# External object files for target historical_linked_list_test
historical_linked_list_test_EXTERNAL_OBJECTS =

bin/historical_linked_list_test: CMakeFiles/historical_linked_list_test.dir/eccc/Core/tests/historical_linked_list_test.cpp.o
bin/historical_linked_list_test: CMakeFiles/historical_linked_list_test.dir/eccc/Core/src/historical_linked_list.cpp.o
bin/historical_linked_list_test: CMakeFiles/historical_linked_list_test.dir/build.make
bin/historical_linked_list_test: CMakeFiles/historical_linked_list_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable bin/historical_linked_list_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/historical_linked_list_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/historical_linked_list_test.dir/build: bin/historical_linked_list_test
.PHONY : CMakeFiles/historical_linked_list_test.dir/build

CMakeFiles/historical_linked_list_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/historical_linked_list_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/historical_linked_list_test.dir/clean

CMakeFiles/historical_linked_list_test.dir/depend:
	cd /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/build /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/build /Users/borismilev/desktop/linked-list-sprint-project-east-coast-coding-crew/build/CMakeFiles/historical_linked_list_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/historical_linked_list_test.dir/depend

