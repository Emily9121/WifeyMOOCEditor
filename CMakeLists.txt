#
# 💖 WifeyMOOC Editor CMake Configuration 💖
# For developers who prefer CMake over qmake! ✨
#

cmake_minimum_required(VERSION 3.16)
project(WifeyMOOCEditor VERSION 2.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable automatic MOC, UIC and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON) 
set(CMAKE_AUTOUIC ON)

set(CMAKE_BUILD_TYPE Debug)

# Source files - all our perfectly fixed editors! ✨
set(SOURCES
    main.cpp
    mainwindow.cpp
    editors/mcqsingleeditor.cpp
    editors/mcqmultipleeditor.cpp
    editors/wordfilleditor.cpp
    editors/imagetaggingeditor.cpp
    editors/matchsentenceeditor.cpp
    editors/multiquestionseditor.cpp
    editors/categorizationeditor.cpp
    editors/fillblanksdropdowneditor.cpp
    editors/listpickeditor.cpp
    editors/matchphraseseditor.cpp
    editors/orderphraseeditor.cpp
    editors/sequenceaudioeditor.cpp
)

# Header files - all the adorable declarations! 💕
set(HEADERS
    mainwindow.h
    helpers.h
    editors/basequestioneditor.h
    editors/mcqsingleeditor.h
    editors/mcqmultipleeditor.h
    editors/wordfilleditor.h
    editors/imagetaggingeditor.h
    editors/matchsentenceeditor.h
    editors/multiquestionseditor.h
    editors/categorizationeditor.h
    editors/fillblanksdropdowneditor.h
    editors/listpickeditor.h
    editors/matchphraseseditor.h
    editors/orderphraseeditor.h
    editors/sequenceaudioeditor.h
)

# Create the executable
add_executable(WifeyMOOCEditor ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(WifeyMOOCEditor Qt6::Core Qt6::Widgets)

# Include directories
target_include_directories(WifeyMOOCEditor PRIVATE . editors)

# Compiler flags for extra safety and cuteness! 🛡️
target_compile_options(WifeyMOOCEditor PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Set application properties
set_target_properties(WifeyMOOCEditor PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_STRING "The cutest quiz editor ever! 💖"
    MACOSX_BUNDLE_BUNDLE_NAME "WifeyMOOC Editor"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
)

# Print cute build message! 💖
message(STATUS "Building the most adorable quiz editor with CMake! 💖✨")
