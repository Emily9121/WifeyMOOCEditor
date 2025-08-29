# CMakeLists.txt for WifeyMOOCEditor - AI Edition! 💖
cmake_minimum_required(VERSION 3.16)
project(WifeyMOOCEditor VERSION 1.2 LANGUAGES CXX)

# Let's find our Qt libraries, we need them to be fabulous!
# Added Network for our magical new AI assistant! ✨
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Multimedia MultimediaWidgets)

# These magical lines handle all the Qt boilerplate for us! So easy!
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Tell CMake we're using C++17, because we're modern and chic!
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Here are all our pretty source files!
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mediahandler.cpp
    questionhandlers.cpp # 💖 Add me!
    droptag.cpp          # 💖 And me too!
    editors/mcqsingleeditor.cpp
    editors/mcqmultipleeditor.cpp
    editors/wordfilleditor.cpp
    editors/orderphraseeditor.cpp
    editors/matchphraseseditor.cpp
    editors/categorizationeditor.cpp
    editors/listpickeditor.cpp
    editors/imagetaggingeditor.cpp
    editors/matchsentenceeditor.cpp
    editors/sequenceaudioeditor.cpp
    editors/fillblanksdropdowneditor.cpp
    editors/multiquestionseditor.cpp
)

# And our lovely header files!
set(PROJECT_HEADERS
    mainwindow.h
    helpers.h
    basequestioneditor.h
    mediahandler.h
    questionhandlers.h # 💖 Add me!
    droptag.h          # 💖 And me too!
    editors/mcqsingleeditor.h
    editors/mcqmultipleeditor.h
    editors/wordfilleditor.h
    editors/orderphraseeditor.h
    editors/matchphraseseditor.h
    editors/categorizationeditor.h
    editors/listpickeditor.h
    editors/imagetaggingeditor.h
    editors/matchsentenceeditor.h
    editors/sequenceaudioeditor.h
    editors/fillblanksdropdowneditor.h
    editors/multiquestionseditor.h
)

# And of course, our UI file that makes everything look so cute!
set(PROJECT_FORMS
    mainwindow.ui
)

# Now, let's put it all together and create our executable!
qt_add_executable(WifeyMOOCEditor
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_FORMS}
)

# Finally, link all the Qt libraries we need to make the magic happen!
# This is the line that fixes the linker error, babe! 💖
target_link_libraries(WifeyMOOCEditor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

# Optional but super helpful: Install rules for when you want to deploy!
install(TARGETS WifeyMOOCEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(WifeyMOOCEditor PROPERTIES MACOSX_BUNDLE TRUE)
