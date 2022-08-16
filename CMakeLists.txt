cmake_minimum_required(VERSION 3.0.0)
project(nnUNetTool VERSION 1.0.0)

cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0043 NEW)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0053 NEW)
cmake_policy(SET CMP0071 NEW)
#cmake_policy(SET CMP0118 OLD)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

# 使用 utf-8 编码
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(Qt5 REQUIRED 
  Core 
  Gui 
  Widgets 
  Multimedia 
  MultimediaWidgets
  Network 
  OpenGL 
  Xml 
  AxContainer 
  PrintSupport
  Concurrent
)


INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/Libs
  ${CMAKE_SOURCE_DIR}/Libs/nlohmann

)

LINK_DIRECTORIES(

)

set(header_files
  nnUNetTool.h
)

set(source_files
  nnUNetTool.cpp
  main.cpp
)

set(ui_files
  nnUNetTool.ui
)

set(qrc_files
 Resources/nnUNetTool.qrc
)

qt5_wrap_cpp(${PROJECT_NAME}_MOC_CXX ${header_files})
qt5_wrap_ui(${PROJECT_NAME}_UI_CXX ${ui_files})
qt5_add_resources(${PROJECT_NAME}_RCC_CXX ${qrc_files})


set(app_WINRC nnUNetTool.rc)
set_property(SOURCE nnUNetTool.rc APPEND PROPERTY
        OBJECT_DEPENDS ${CMAKE_SOURCE_DIR}/Resources/icon.ico
        )


add_executable(${PROJECT_NAME} WIN32
  ${header_files}
  ${source_files}
  ${app_WINRC}
  ${${PROJECT_NAME}_MOC_CXX}
  ${${PROJECT_NAME}_UI_CXX}
  ${${PROJECT_NAME}_RCC_CXX}
)


if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if(NOT show_console_debug)
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true)
  endif()
endif()

target_link_libraries(${PROJECT_NAME}
  Qt5::Core 
  Qt5::Gui 
  Qt5::Widgets
  Qt5::Multimedia
  Qt5::Network
)

#if(USE_SPDLOG)
#  target_link_libraries(${PROJECT_NAME} 
#    spdlog::spdlog
#  )
#endif(USE_SPDLOG)



if(DEFINED ENV{VisualWorkspace})
	set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$ENV{VisualWorkspace}")
endif()