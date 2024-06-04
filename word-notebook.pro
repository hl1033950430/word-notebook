QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    input_name_window.cpp \
    main.cpp \
    mainwindow.cpp \
    notebook_editor.cpp \
    notebook_list_window.cpp \
    txt_notebook_editor.cpp \
    worditem.cpp

HEADERS += \
    input_name_window.h \
    mainwindow.h \
    notebook_editor.h \
    notebook_list_window.h \
    txt_notebook_editor.h \
    worditem.h

FORMS += \
    input_name_window.ui \
    mainwindow.ui \
    notebook_list_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
