QT += core gui widgets

CONFIG += c++17

TARGET = WifeyMOOCEditor
TEMPLATE = app

HEADERS += \
    mainwindow.h \
    basequestioneditor.h \
    helpers.h \
    editors/mcqsingleeditor.h \
    editors/mcqmultipleeditor.h \
    editors/listpickeditor.h \
    editors/wordfilleditor.h \
    editors/matchsentenceeditor.h \
    editors/orderphraseeditor.h \
    editors/categorizationeditor.h \
    editors/fillblanksdropdowneditor.h \
    editors/sequenceaudioeditor.h \
    editors/matchphraseseditor.h \
    editors/multiquestionseditor.h \
    editors/imagetaggingeditor.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    editors/mcqsingleeditor.cpp \
    editors/mcqmultipleeditor.cpp \
    editors/listpickeditor.cpp \
    editors/wordfilleditor.cpp \
    editors/matchsentenceeditor.cpp \
    editors/orderphraseeditor.cpp \
    editors/categorizationeditor.cpp \
    editors/fillblanksdropdowneditor.cpp \
    editors/sequenceaudioeditor.cpp \
    editors/matchphraseseditor.cpp \
    editors/multiquestionseditor.cpp \
    editors/imagetaggingeditor.cpp

# For macOS, this creates a nice .app bundle
macx {
    ICON = app_icon.icns
}