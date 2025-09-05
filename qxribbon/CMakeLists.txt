cmake_minimum_required(VERSION 3.16)
project(qxribbon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# 先用 GLOB 跑通（后续建议手工列出）
file(GLOB_RECURSE QXRIBBON_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/private/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.qrc"


)

add_library(qxribbon ${QXRIBBON_SOURCES})
add_library(qxribbon::qxribbon ALIAS qxribbon)

# 与 qxribbon_global.h 中的导出宏匹配
if(BUILD_SHARED_LIBS)
  target_compile_definitions(qxribbon PRIVATE QX_RIBBON_LIBRARY)          # 导出 DLL
else()
  target_compile_definitions(qxribbon PUBLIC  QX_RIBBON_LIBRARY_STATIC)   # 静态库
endif()

target_include_directories(qxribbon
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(qxribbon
  PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    # 如果 qxribbon 源码中直接包含/调用了 qxwindow，请取消下一行注释：
    # qxwindow
)

set_target_properties(qxribbon PROPERTIES POSITION_INDEPENDENT_CODE ON)
