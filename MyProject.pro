#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T02:57:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    pointwidget.cpp \
    topocoord.cpp \
    normalmatrixform.cpp \
    korellmatrixofvector.cpp \
    vectparam.cpp

HEADERS  += mainwindow.h \
    main.h \
    pointwidget.h \
    topocoord.h \
    normalmatrixform.h \
    korellmatrixofvector.h \
    vectparam.h

FORMS    += mainwindow.ui \
    pointwidget.ui \
    topocoord.ui \
    normalmatrixform.ui \
    korellmatrixofvector.ui \
    vectparam.ui
