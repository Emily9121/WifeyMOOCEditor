# ----------------------------------------------------
# This is the project file for our super cute editor!
# It tells Qt which files to build. It's very important!
# ----------------------------------------------------

QT       += core gui
QT       += widgets

CONFIG   += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# The name of the final executable
TARGET = WifeyMOOCEditor

# All the C++ source files that are part of the project
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    editors/categorizationeditor.cpp \
    editors/fillblanksdropdowneditor.cpp \
    editors/imagetaggingeditor.cpp \
    editors/listpickeditor.cpp \
    editors/matchphraseseditor.cpp \
    editors/matchsentenceeditor.cpp \
    editors/mcqmultipleeditor.cpp \
    editors/mcqsingleeditor.cpp \
    editors/multiquestionseditor.cpp \
    editors/orderphraseeditor.cpp \
    editors/sequenceaudioeditor.cpp \
    editors/wordfilleditor.cpp

# All the header files that are part of the project
HEADERS += \
    mainwindow.h \
    helpers.h \
    basequestioneditor.h \
    editors/categorizationeditor.h \
    editors/fillblanksdropdowneditor.h \
    editors/imagetaggingeditor.h \
    editors/listpickeditor.h \
    editors/matchphraseseditor.h \
    editors/matchsentenceeditor.h \
    editors/mcqmultipleeditor.h \
    editors/mcqsingleeditor.h \
    editors/multiquestionseditor.h \
    editors/orderphraseeditor.h \
    editors/sequenceaudioeditor.h \
    editors/wordfilleditor.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
