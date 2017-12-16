#-------------------------------------------------
#
# Project created by QtCreator 2017-09-15T15:52:13
#
#-------------------------------------------------

QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mybrowser
TEMPLATE = app


SOURCES += main.cpp\
    urllineedit.cpp \
    htmlhighlighter.cpp \
    popupmenu.cpp \
    mysettings.cpp \
    mymainwindow.cpp

HEADERS  += \
    urllineedit.h \
    htmlhighlighter.h \
    popupmenu.h \
    mysettings.h \
    mymainwindow.h

TRANSLATIONS += lang_ru.ts

FORMS +=

RESOURCES += \
    myresources.qrc
