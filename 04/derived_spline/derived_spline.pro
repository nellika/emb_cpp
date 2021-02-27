#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T21:40:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = derived_spline
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    splineimage.cpp


HEADERS  += mainwindow.h \
    elec4_util.h \
    splineimage.h

QMAKE_CXXFLAGS += -std=c++14 -mavx
INCLUDEPATH += /usr/local/include/eigen/

