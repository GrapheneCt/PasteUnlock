cmake_minimum_required(VERSION 3.19)

set(CMAKE_TOOLCHAIN_FILE "$ENV{SCE_PSP2_SDK_DIR}/host_tools/build/cmake/psp2-snc-toolchain.cmake")

project(PasteUnlock)

include(VitaDevelopmentSuite)
enable_language(EMD)
set(CMAKE_C_STANDARD 99)

add_compile_options(
  -Xdiag=0 -Xquit=2 -O3
)

add_link_options(
  --prx-no-runtime-support
  --strip-unused-data
  --strip-duplicates
  --strip-report=strip-report.txt
)

include_directories(
  ${VDSUITE_USER_INCLUDE_DIRECTORIES}
  ${VDSUITE_COMMON_INCLUDE_DIRECTORIES}
)

link_directories(
  ${VDSUITE_LIBRARY_DIRECTORIES}
)

add_library("${PROJECT_NAME}.elf" MODULE
  main.c
  lib.emd
)

target_link_libraries("${PROJECT_NAME}.elf"
  SceLibKernel_stub
  SceThreadMgr_stub
  SceModulemgr_stub
  taihen_stub
)

VDSuiteSignElf("${PROJECT_NAME}.suprx" "${PROJECT_NAME}.elf" COMPRESS ON STRIP ON)