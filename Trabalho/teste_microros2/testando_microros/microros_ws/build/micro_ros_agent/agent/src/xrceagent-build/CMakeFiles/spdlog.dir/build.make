# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build

# Utility rule file for spdlog.

# Include any custom commands dependencies for this target.
include CMakeFiles/spdlog.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/spdlog.dir/progress.make

CMakeFiles/spdlog: CMakeFiles/spdlog-complete

CMakeFiles/spdlog-complete: spdlog/src/spdlog-stamp/spdlog-install
CMakeFiles/spdlog-complete: spdlog/src/spdlog-stamp/spdlog-mkdir
CMakeFiles/spdlog-complete: spdlog/src/spdlog-stamp/spdlog-download
CMakeFiles/spdlog-complete: spdlog/src/spdlog-stamp/spdlog-update
CMakeFiles/spdlog-complete: spdlog/src/spdlog-stamp/spdlog-patch
CMakeFiles/spdlog-complete: spdlog/src/spdlog-stamp/spdlog-configure
CMakeFiles/spdlog-complete: spdlog/src/spdlog-stamp/spdlog-build
CMakeFiles/spdlog-complete: spdlog/src/spdlog-stamp/spdlog-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'spdlog'"
	/usr/bin/cmake -E make_directory /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles
	/usr/bin/cmake -E touch /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles/spdlog-complete
	/usr/bin/cmake -E touch /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-stamp/spdlog-done

spdlog/src/spdlog-stamp/spdlog-build: spdlog/src/spdlog-stamp/spdlog-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'spdlog'"
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-build && $(MAKE)
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-build && /usr/bin/cmake -E touch /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-stamp/spdlog-build

spdlog/src/spdlog-stamp/spdlog-configure: spdlog/tmp/spdlog-cfgcmd.txt
spdlog/src/spdlog-stamp/spdlog-configure: spdlog/tmp/spdlog-cache-.cmake
spdlog/src/spdlog-stamp/spdlog-configure: spdlog/src/spdlog-stamp/spdlog-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'spdlog'"
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-build && /usr/bin/cmake "-GUnix Makefiles" -C/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/tmp/spdlog-cache-.cmake -S /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog -B /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-build
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-build && /usr/bin/cmake -E touch /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-stamp/spdlog-configure

spdlog/src/spdlog-stamp/spdlog-download: spdlog/src/spdlog-stamp/spdlog-gitinfo.txt
spdlog/src/spdlog-stamp/spdlog-download: spdlog/src/spdlog-stamp/spdlog-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'spdlog'"
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src && /usr/bin/cmake -P /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/tmp/spdlog-gitclone.cmake
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src && /usr/bin/cmake -E touch /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-stamp/spdlog-download

spdlog/src/spdlog-stamp/spdlog-install: spdlog/src/spdlog-stamp/spdlog-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Performing install step for 'spdlog'"
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-build && $(MAKE) install
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-build && /usr/bin/cmake -E touch /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-stamp/spdlog-install

spdlog/src/spdlog-stamp/spdlog-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'spdlog'"
	/usr/bin/cmake -Dcfgdir= -P /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/tmp/spdlog-mkdirs.cmake
	/usr/bin/cmake -E touch /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-stamp/spdlog-mkdir

spdlog/src/spdlog-stamp/spdlog-patch: spdlog/src/spdlog-stamp/spdlog-patch-info.txt
spdlog/src/spdlog-stamp/spdlog-patch: spdlog/src/spdlog-stamp/spdlog-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'spdlog'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog-stamp/spdlog-patch

spdlog/src/spdlog-stamp/spdlog-update: spdlog/tmp/spdlog-gitupdate.cmake
spdlog/src/spdlog-stamp/spdlog-update: spdlog/src/spdlog-stamp/spdlog-update-info.txt
spdlog/src/spdlog-stamp/spdlog-update: spdlog/src/spdlog-stamp/spdlog-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing update step for 'spdlog'"
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/src/spdlog && /usr/bin/cmake -Dcan_fetch=YES -P /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/spdlog/tmp/spdlog-gitupdate.cmake

spdlog: CMakeFiles/spdlog
spdlog: CMakeFiles/spdlog-complete
spdlog: spdlog/src/spdlog-stamp/spdlog-build
spdlog: spdlog/src/spdlog-stamp/spdlog-configure
spdlog: spdlog/src/spdlog-stamp/spdlog-download
spdlog: spdlog/src/spdlog-stamp/spdlog-install
spdlog: spdlog/src/spdlog-stamp/spdlog-mkdir
spdlog: spdlog/src/spdlog-stamp/spdlog-patch
spdlog: spdlog/src/spdlog-stamp/spdlog-update
spdlog: CMakeFiles/spdlog.dir/build.make
.PHONY : spdlog

# Rule to build all files generated by this target.
CMakeFiles/spdlog.dir/build: spdlog
.PHONY : CMakeFiles/spdlog.dir/build

CMakeFiles/spdlog.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/spdlog.dir/cmake_clean.cmake
.PHONY : CMakeFiles/spdlog.dir/clean

CMakeFiles/spdlog.dir/depend:
	cd /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build /home/ivan/Desktop/Unicamp/IvanUnicamp/Disciplinas/IM_420_Sistemas_Embarcados_de_Tempo_Real/Trabalho/teste_microros2/testando_microros/microros_ws/build/micro_ros_agent/agent/src/xrceagent-build/CMakeFiles/spdlog.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/spdlog.dir/depend

