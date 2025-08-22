cmake_minimum_required(VERSION 3.16)

project(WifeyMOOCEditor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 setup
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# Source files
set(SOURCES
    main.cpp
    mainwindow.cpp
    questioneditor.cpp
    configdialog.cpp
    questiontemplate.cpp
    jsonhandler.cpp
)

set(HEADERS
    mainwindow.h
    questioneditor.h
    configdialog.h
    questiontemplate.h
    jsonhandler.h
    questiontypes.h
)

# Create executable
qt_add_executable(WifeyMOOCEditor ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(WifeyMOOCEditor PRIVATE Qt6::Core Qt6::Widgets)

# Set application properties
set_target_properties(WifeyMOOCEditor PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Copy resources to build directory
configure_file(${CMAKE_SOURCE_DIR}/README.md ${CMAKE_BINARY_DIR} COPYONLY)
install(TARGETS WifeyMOOCEditor DESTINATION bin)
