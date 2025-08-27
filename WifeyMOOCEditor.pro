#
# 💖 WifeyMOOC Editor C++ Edition Project File 💖
# Created with love and lots of pink! ✨
# 

QT += core widgets

CONFIG += c++17
CONFIG += console

TARGET = WifeyMOOCEditor
TEMPLATE = app

# Define the application version
VERSION = 2.1.0

# Application info
QMAKE_TARGET_COMPANY = "Emily's Adorable Software"
QMAKE_TARGET_PRODUCT = "WifeyMOOC JSON Editor"
QMAKE_TARGET_DESCRIPTION = "The cutest quiz editor ever created! 💖"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2025 Emily"

# Source files - all our perfectly fixed editors! ✨
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    editors/basequestioneditor.cpp \
    editors/mcqsingleeditor.cpp \
    editors/mcqmultipleeditor.cpp \
    editors/wordfilleditor.cpp \
    editors/imagetaggingeditor.cpp \
    editors/matchsentenceeditor.cpp \
    editors/multiquestionseditor.cpp \
    editors/categorizationeditor.cpp \
    editors/fillblanksdropdowneditor.cpp \
    editors/listpickeditor.cpp \
    editors/matchphraseseditor.cpp \
    editors/orderphraseeditor.cpp \
    editors/sequenceaudioeditor.cpp

# Header files - all the adorable declarations! 💕
HEADERS += \
    mainwindow.h \
    helpers.h \
    editors/basequestioneditor.h \
    editors/mcqsingleeditor.h \
    editors/mcqmultipleeditor.h \
    editors/wordfilleditor.h \
    editors/imagetaggingeditor.h \
    editors/matchsentenceeditor.h \
    editors/multiquestionseditor.h \
    editors/categorizationeditor.h \
    editors/fillblanksdropdowneditor.h \
    editors/listpickeditor.h \
    editors/matchphraseseditor.h \
    editors/orderphraseeditor.h \
    editors/sequenceaudioeditor.h

# Include paths for our cute headers
INCLUDEPATH += . editors

# Build configuration - make it perfect! ✨
CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
    RCC_DIR = debug/rcc
} else {
    DESTDIR = release
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
    RCC_DIR = release/rcc
}

# Compiler flags for extra cuteness and safety! 🛡️
QMAKE_CXXFLAGS += -Wall -Wextra

# Platform-specific settings
win32 {
    # Windows-specific settings
    CONFIG += windows
    QMAKE_CXXFLAGS += -std=c++17
}

unix:!macx {
    # Linux-specific settings
    QMAKE_CXXFLAGS += -std=c++17
}

macx {
    # macOS-specific settings - make it extra pretty on Mac! 🍎
    QMAKE_CXXFLAGS += -std=c++17
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14

    # Bundle configuration
    QMAKE_INFO_PLIST = Info.plist
    ICON = wifeymooc_icon.icns
}

# Installation paths (optional)
target.path = /usr/local/bin
INSTALLS += target

# Define preprocessor macros
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_VERSION=\"$$VERSION\"

# Enable all Qt modules we need
QT += gui widgets core

# Make sure we use the correct Qt version
lessThan(QT_MAJOR_VERSION, 5): error("This project requires Qt 5.0 or later")

# Resource files (if you have icons, etc.)
# RESOURCES += resources.qrc

# Translations (if you want multi-language support)
# TRANSLATIONS += translations/wifeymooc_en.ts translations/wifeymooc_fr.ts

# Additional clean files
QMAKE_CLEAN += $(TARGET)

# Print a cute message during build! 💖
message("Building the most adorable quiz editor ever! 💖✨")
