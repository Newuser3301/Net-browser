cmake_minimum_required(VERSION 3.20)
project(netbrowser_core CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(netbrowser_core
  src/core/browser/app/browser_main.cc
  src/core/browser/app/browser_process.cc
  src/ipc/transport/named_pipes_win.cc
  src/platform/c_shim/win/net_platform_win.c
)

target_include_directories(netbrowser_core PRIVATE
  ${CMAKE_SOURCE_DIR}
)

if (WIN32)
  target_compile_definitions(netbrowser_core PRIVATE _WIN32)
endif()