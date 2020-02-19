# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/chris/AntTracking/bb_imgacquisition_tim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/AntTracking/bb_imgacquisition_tim

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/cmake-gui -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/chris/AntTracking/bb_imgacquisition_tim/CMakeFiles /home/chris/AntTracking/bb_imgacquisition_tim/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/chris/AntTracking/bb_imgacquisition_tim/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named bb_ImageAcquistion

# Build rule for target.
bb_ImageAcquistion: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 bb_ImageAcquistion
.PHONY : bb_ImageAcquistion

# fast build rule for target.
bb_ImageAcquistion/fast:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/build
.PHONY : bb_ImageAcquistion/fast

#=============================================================================
# Target rules for targets named bb_ImageAcquistion_autogen

# Build rule for target.
bb_ImageAcquistion_autogen: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 bb_ImageAcquistion_autogen
.PHONY : bb_ImageAcquistion_autogen

# fast build rule for target.
bb_ImageAcquistion_autogen/fast:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion_autogen.dir/build.make CMakeFiles/bb_ImageAcquistion_autogen.dir/build
.PHONY : bb_ImageAcquistion_autogen/fast

BaslerCamThread.o: BaslerCamThread.cpp.o

.PHONY : BaslerCamThread.o

# target to build an object file
BaslerCamThread.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/BaslerCamThread.cpp.o
.PHONY : BaslerCamThread.cpp.o

BaslerCamThread.i: BaslerCamThread.cpp.i

.PHONY : BaslerCamThread.i

# target to preprocess a source file
BaslerCamThread.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/BaslerCamThread.cpp.i
.PHONY : BaslerCamThread.cpp.i

BaslerCamThread.s: BaslerCamThread.cpp.s

.PHONY : BaslerCamThread.s

# target to generate assembly for a file
BaslerCamThread.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/BaslerCamThread.cpp.s
.PHONY : BaslerCamThread.cpp.s

Buffer/MutexLinkedList.o: Buffer/MutexLinkedList.cpp.o

.PHONY : Buffer/MutexLinkedList.o

# target to build an object file
Buffer/MutexLinkedList.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/Buffer/MutexLinkedList.cpp.o
.PHONY : Buffer/MutexLinkedList.cpp.o

Buffer/MutexLinkedList.i: Buffer/MutexLinkedList.cpp.i

.PHONY : Buffer/MutexLinkedList.i

# target to preprocess a source file
Buffer/MutexLinkedList.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/Buffer/MutexLinkedList.cpp.i
.PHONY : Buffer/MutexLinkedList.cpp.i

Buffer/MutexLinkedList.s: Buffer/MutexLinkedList.cpp.s

.PHONY : Buffer/MutexLinkedList.s

# target to generate assembly for a file
Buffer/MutexLinkedList.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/Buffer/MutexLinkedList.cpp.s
.PHONY : Buffer/MutexLinkedList.cpp.s

CamThread.o: CamThread.cpp.o

.PHONY : CamThread.o

# target to build an object file
CamThread.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/CamThread.cpp.o
.PHONY : CamThread.cpp.o

CamThread.i: CamThread.cpp.i

.PHONY : CamThread.i

# target to preprocess a source file
CamThread.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/CamThread.cpp.i
.PHONY : CamThread.cpp.i

CamThread.s: CamThread.cpp.s

.PHONY : CamThread.s

# target to generate assembly for a file
CamThread.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/CamThread.cpp.s
.PHONY : CamThread.cpp.s

ImageAnalysis.o: ImageAnalysis.cpp.o

.PHONY : ImageAnalysis.o

# target to build an object file
ImageAnalysis.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/ImageAnalysis.cpp.o
.PHONY : ImageAnalysis.cpp.o

ImageAnalysis.i: ImageAnalysis.cpp.i

.PHONY : ImageAnalysis.i

# target to preprocess a source file
ImageAnalysis.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/ImageAnalysis.cpp.i
.PHONY : ImageAnalysis.cpp.i

ImageAnalysis.s: ImageAnalysis.cpp.s

.PHONY : ImageAnalysis.s

# target to generate assembly for a file
ImageAnalysis.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/ImageAnalysis.cpp.s
.PHONY : ImageAnalysis.cpp.s

ImgAcquisitionApp.o: ImgAcquisitionApp.cpp.o

.PHONY : ImgAcquisitionApp.o

# target to build an object file
ImgAcquisitionApp.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/ImgAcquisitionApp.cpp.o
.PHONY : ImgAcquisitionApp.cpp.o

ImgAcquisitionApp.i: ImgAcquisitionApp.cpp.i

.PHONY : ImgAcquisitionApp.i

# target to preprocess a source file
ImgAcquisitionApp.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/ImgAcquisitionApp.cpp.i
.PHONY : ImgAcquisitionApp.cpp.i

ImgAcquisitionApp.s: ImgAcquisitionApp.cpp.s

.PHONY : ImgAcquisitionApp.s

# target to generate assembly for a file
ImgAcquisitionApp.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/ImgAcquisitionApp.cpp.s
.PHONY : ImgAcquisitionApp.cpp.s

NvEncGlue.o: NvEncGlue.cpp.o

.PHONY : NvEncGlue.o

# target to build an object file
NvEncGlue.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/NvEncGlue.cpp.o
.PHONY : NvEncGlue.cpp.o

NvEncGlue.i: NvEncGlue.cpp.i

.PHONY : NvEncGlue.i

# target to preprocess a source file
NvEncGlue.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/NvEncGlue.cpp.i
.PHONY : NvEncGlue.cpp.i

NvEncGlue.s: NvEncGlue.cpp.s

.PHONY : NvEncGlue.s

# target to generate assembly for a file
NvEncGlue.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/NvEncGlue.cpp.s
.PHONY : NvEncGlue.cpp.s

SharedMemory.o: SharedMemory.cpp.o

.PHONY : SharedMemory.o

# target to build an object file
SharedMemory.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/SharedMemory.cpp.o
.PHONY : SharedMemory.cpp.o

SharedMemory.i: SharedMemory.cpp.i

.PHONY : SharedMemory.i

# target to preprocess a source file
SharedMemory.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/SharedMemory.cpp.i
.PHONY : SharedMemory.cpp.i

SharedMemory.s: SharedMemory.cpp.s

.PHONY : SharedMemory.s

# target to generate assembly for a file
SharedMemory.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/SharedMemory.cpp.s
.PHONY : SharedMemory.cpp.s

bb_ImageAcquistion_autogen/mocs_compilation.o: bb_ImageAcquistion_autogen/mocs_compilation.cpp.o

.PHONY : bb_ImageAcquistion_autogen/mocs_compilation.o

# target to build an object file
bb_ImageAcquistion_autogen/mocs_compilation.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/bb_ImageAcquistion_autogen/mocs_compilation.cpp.o
.PHONY : bb_ImageAcquistion_autogen/mocs_compilation.cpp.o

bb_ImageAcquistion_autogen/mocs_compilation.i: bb_ImageAcquistion_autogen/mocs_compilation.cpp.i

.PHONY : bb_ImageAcquistion_autogen/mocs_compilation.i

# target to preprocess a source file
bb_ImageAcquistion_autogen/mocs_compilation.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/bb_ImageAcquistion_autogen/mocs_compilation.cpp.i
.PHONY : bb_ImageAcquistion_autogen/mocs_compilation.cpp.i

bb_ImageAcquistion_autogen/mocs_compilation.s: bb_ImageAcquistion_autogen/mocs_compilation.cpp.s

.PHONY : bb_ImageAcquistion_autogen/mocs_compilation.s

# target to generate assembly for a file
bb_ImageAcquistion_autogen/mocs_compilation.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/bb_ImageAcquistion_autogen/mocs_compilation.cpp.s
.PHONY : bb_ImageAcquistion_autogen/mocs_compilation.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/main.cpp.s
.PHONY : main.cpp.s

nvenc/NvEncoder.o: nvenc/NvEncoder.cpp.o

.PHONY : nvenc/NvEncoder.o

# target to build an object file
nvenc/NvEncoder.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/NvEncoder.cpp.o
.PHONY : nvenc/NvEncoder.cpp.o

nvenc/NvEncoder.i: nvenc/NvEncoder.cpp.i

.PHONY : nvenc/NvEncoder.i

# target to preprocess a source file
nvenc/NvEncoder.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/NvEncoder.cpp.i
.PHONY : nvenc/NvEncoder.cpp.i

nvenc/NvEncoder.s: nvenc/NvEncoder.cpp.s

.PHONY : nvenc/NvEncoder.s

# target to generate assembly for a file
nvenc/NvEncoder.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/NvEncoder.cpp.s
.PHONY : nvenc/NvEncoder.cpp.s

nvenc/NvHWEncoder.o: nvenc/NvHWEncoder.cpp.o

.PHONY : nvenc/NvHWEncoder.o

# target to build an object file
nvenc/NvHWEncoder.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/NvHWEncoder.cpp.o
.PHONY : nvenc/NvHWEncoder.cpp.o

nvenc/NvHWEncoder.i: nvenc/NvHWEncoder.cpp.i

.PHONY : nvenc/NvHWEncoder.i

# target to preprocess a source file
nvenc/NvHWEncoder.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/NvHWEncoder.cpp.i
.PHONY : nvenc/NvHWEncoder.cpp.i

nvenc/NvHWEncoder.s: nvenc/NvHWEncoder.cpp.s

.PHONY : nvenc/NvHWEncoder.s

# target to generate assembly for a file
nvenc/NvHWEncoder.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/NvHWEncoder.cpp.s
.PHONY : nvenc/NvHWEncoder.cpp.s

nvenc/cudaModuleMgr.o: nvenc/cudaModuleMgr.cpp.o

.PHONY : nvenc/cudaModuleMgr.o

# target to build an object file
nvenc/cudaModuleMgr.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/cudaModuleMgr.cpp.o
.PHONY : nvenc/cudaModuleMgr.cpp.o

nvenc/cudaModuleMgr.i: nvenc/cudaModuleMgr.cpp.i

.PHONY : nvenc/cudaModuleMgr.i

# target to preprocess a source file
nvenc/cudaModuleMgr.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/cudaModuleMgr.cpp.i
.PHONY : nvenc/cudaModuleMgr.cpp.i

nvenc/cudaModuleMgr.s: nvenc/cudaModuleMgr.cpp.s

.PHONY : nvenc/cudaModuleMgr.s

# target to generate assembly for a file
nvenc/cudaModuleMgr.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/cudaModuleMgr.cpp.s
.PHONY : nvenc/cudaModuleMgr.cpp.s

nvenc/dynlink_cuda.o: nvenc/dynlink_cuda.cpp.o

.PHONY : nvenc/dynlink_cuda.o

# target to build an object file
nvenc/dynlink_cuda.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/dynlink_cuda.cpp.o
.PHONY : nvenc/dynlink_cuda.cpp.o

nvenc/dynlink_cuda.i: nvenc/dynlink_cuda.cpp.i

.PHONY : nvenc/dynlink_cuda.i

# target to preprocess a source file
nvenc/dynlink_cuda.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/dynlink_cuda.cpp.i
.PHONY : nvenc/dynlink_cuda.cpp.i

nvenc/dynlink_cuda.s: nvenc/dynlink_cuda.cpp.s

.PHONY : nvenc/dynlink_cuda.s

# target to generate assembly for a file
nvenc/dynlink_cuda.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/dynlink_cuda.cpp.s
.PHONY : nvenc/dynlink_cuda.cpp.s

nvenc/dynlink_nvcuvid.o: nvenc/dynlink_nvcuvid.cpp.o

.PHONY : nvenc/dynlink_nvcuvid.o

# target to build an object file
nvenc/dynlink_nvcuvid.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/dynlink_nvcuvid.cpp.o
.PHONY : nvenc/dynlink_nvcuvid.cpp.o

nvenc/dynlink_nvcuvid.i: nvenc/dynlink_nvcuvid.cpp.i

.PHONY : nvenc/dynlink_nvcuvid.i

# target to preprocess a source file
nvenc/dynlink_nvcuvid.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/dynlink_nvcuvid.cpp.i
.PHONY : nvenc/dynlink_nvcuvid.cpp.i

nvenc/dynlink_nvcuvid.s: nvenc/dynlink_nvcuvid.cpp.s

.PHONY : nvenc/dynlink_nvcuvid.s

# target to generate assembly for a file
nvenc/dynlink_nvcuvid.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/nvenc/dynlink_nvcuvid.cpp.s
.PHONY : nvenc/dynlink_nvcuvid.cpp.s

settings/Settings.o: settings/Settings.cpp.o

.PHONY : settings/Settings.o

# target to build an object file
settings/Settings.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/Settings.cpp.o
.PHONY : settings/Settings.cpp.o

settings/Settings.i: settings/Settings.cpp.i

.PHONY : settings/Settings.i

# target to preprocess a source file
settings/Settings.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/Settings.cpp.i
.PHONY : settings/Settings.cpp.i

settings/Settings.s: settings/Settings.cpp.s

.PHONY : settings/Settings.s

# target to generate assembly for a file
settings/Settings.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/Settings.cpp.s
.PHONY : settings/Settings.cpp.s

settings/StringTranslator.o: settings/StringTranslator.cpp.o

.PHONY : settings/StringTranslator.o

# target to build an object file
settings/StringTranslator.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/StringTranslator.cpp.o
.PHONY : settings/StringTranslator.cpp.o

settings/StringTranslator.i: settings/StringTranslator.cpp.i

.PHONY : settings/StringTranslator.i

# target to preprocess a source file
settings/StringTranslator.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/StringTranslator.cpp.i
.PHONY : settings/StringTranslator.cpp.i

settings/StringTranslator.s: settings/StringTranslator.cpp.s

.PHONY : settings/StringTranslator.s

# target to generate assembly for a file
settings/StringTranslator.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/StringTranslator.cpp.s
.PHONY : settings/StringTranslator.cpp.s

settings/stringTools.o: settings/stringTools.cpp.o

.PHONY : settings/stringTools.o

# target to build an object file
settings/stringTools.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/stringTools.cpp.o
.PHONY : settings/stringTools.cpp.o

settings/stringTools.i: settings/stringTools.cpp.i

.PHONY : settings/stringTools.i

# target to preprocess a source file
settings/stringTools.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/stringTools.cpp.i
.PHONY : settings/stringTools.cpp.i

settings/stringTools.s: settings/stringTools.cpp.s

.PHONY : settings/stringTools.s

# target to generate assembly for a file
settings/stringTools.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/stringTools.cpp.s
.PHONY : settings/stringTools.cpp.s

settings/utility.o: settings/utility.cpp.o

.PHONY : settings/utility.o

# target to build an object file
settings/utility.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/utility.cpp.o
.PHONY : settings/utility.cpp.o

settings/utility.i: settings/utility.cpp.i

.PHONY : settings/utility.i

# target to preprocess a source file
settings/utility.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/utility.cpp.i
.PHONY : settings/utility.cpp.i

settings/utility.s: settings/utility.cpp.s

.PHONY : settings/utility.s

# target to generate assembly for a file
settings/utility.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/settings/utility.cpp.s
.PHONY : settings/utility.cpp.s

writeHandler.o: writeHandler.cpp.o

.PHONY : writeHandler.o

# target to build an object file
writeHandler.cpp.o:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/writeHandler.cpp.o
.PHONY : writeHandler.cpp.o

writeHandler.i: writeHandler.cpp.i

.PHONY : writeHandler.i

# target to preprocess a source file
writeHandler.cpp.i:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/writeHandler.cpp.i
.PHONY : writeHandler.cpp.i

writeHandler.s: writeHandler.cpp.s

.PHONY : writeHandler.s

# target to generate assembly for a file
writeHandler.cpp.s:
	$(MAKE) -f CMakeFiles/bb_ImageAcquistion.dir/build.make CMakeFiles/bb_ImageAcquistion.dir/writeHandler.cpp.s
.PHONY : writeHandler.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... bb_ImageAcquistion"
	@echo "... edit_cache"
	@echo "... bb_ImageAcquistion_autogen"
	@echo "... BaslerCamThread.o"
	@echo "... BaslerCamThread.i"
	@echo "... BaslerCamThread.s"
	@echo "... Buffer/MutexLinkedList.o"
	@echo "... Buffer/MutexLinkedList.i"
	@echo "... Buffer/MutexLinkedList.s"
	@echo "... CamThread.o"
	@echo "... CamThread.i"
	@echo "... CamThread.s"
	@echo "... ImageAnalysis.o"
	@echo "... ImageAnalysis.i"
	@echo "... ImageAnalysis.s"
	@echo "... ImgAcquisitionApp.o"
	@echo "... ImgAcquisitionApp.i"
	@echo "... ImgAcquisitionApp.s"
	@echo "... NvEncGlue.o"
	@echo "... NvEncGlue.i"
	@echo "... NvEncGlue.s"
	@echo "... SharedMemory.o"
	@echo "... SharedMemory.i"
	@echo "... SharedMemory.s"
	@echo "... bb_ImageAcquistion_autogen/mocs_compilation.o"
	@echo "... bb_ImageAcquistion_autogen/mocs_compilation.i"
	@echo "... bb_ImageAcquistion_autogen/mocs_compilation.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... nvenc/NvEncoder.o"
	@echo "... nvenc/NvEncoder.i"
	@echo "... nvenc/NvEncoder.s"
	@echo "... nvenc/NvHWEncoder.o"
	@echo "... nvenc/NvHWEncoder.i"
	@echo "... nvenc/NvHWEncoder.s"
	@echo "... nvenc/cudaModuleMgr.o"
	@echo "... nvenc/cudaModuleMgr.i"
	@echo "... nvenc/cudaModuleMgr.s"
	@echo "... nvenc/dynlink_cuda.o"
	@echo "... nvenc/dynlink_cuda.i"
	@echo "... nvenc/dynlink_cuda.s"
	@echo "... nvenc/dynlink_nvcuvid.o"
	@echo "... nvenc/dynlink_nvcuvid.i"
	@echo "... nvenc/dynlink_nvcuvid.s"
	@echo "... settings/Settings.o"
	@echo "... settings/Settings.i"
	@echo "... settings/Settings.s"
	@echo "... settings/StringTranslator.o"
	@echo "... settings/StringTranslator.i"
	@echo "... settings/StringTranslator.s"
	@echo "... settings/stringTools.o"
	@echo "... settings/stringTools.i"
	@echo "... settings/stringTools.s"
	@echo "... settings/utility.o"
	@echo "... settings/utility.i"
	@echo "... settings/utility.s"
	@echo "... writeHandler.o"
	@echo "... writeHandler.i"
	@echo "... writeHandler.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

