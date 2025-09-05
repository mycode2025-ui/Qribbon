cmake_minimum_required(VERSION 3.16)
project(ribbondemo_standalone LANGUAGES CXX)

# 切换库类型：OFF=静态(默认)，ON=动态(DLL)
option(BUILD_SHARED_LIBS "Build shared libs (ON=DLL, OFF=static)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 兼容 Qt6/Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# 子库
add_subdirectory(qxribbon)

# Demo 源码（建议后续改为手工列出，先用 GLOB 跑通）
file(GLOB RIBBONDEMO_SRCS
  ribbondemo/*.cpp
  ribbondemo/*.h
  ribbondemo/*.ui
  ribbondemo/*.qrc
)

# WIN32 关键字 = GUI 子系统（跨 MSVC/MinGW）
add_executable(ribbondemo WIN32 ${RIBBONDEMO_SRCS})

target_link_libraries(ribbondemo
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    qxribbon
)

# 让 #include "qxwindow/..." / "qxribbon/..." 生效
target_include_directories(ribbondemo PRIVATE
  ${CMAKE_SOURCE_DIR}
)

# 可选：更严格的警告
if(MSVC)
  target_compile_options(ribbondemo PRIVATE /W3)
else()
  target_compile_options(ribbondemo PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
