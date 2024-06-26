#-------------------------------------------------
#
# Project created by QtCreator 2021-02-27T13:30:11
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT       += testlib
QT       += network
QT       += core gui serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Car_Link
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ccd_deal_img.cpp \
    direction.cpp \
    go.cpp \
        main.cpp \
    myslider.cpp \
    speed.cpp \
    stack.cpp \
    udp_thread.cpp \
        widget.cpp \
    deal_img.cpp

HEADERS += \
    ccd_deal_img.h \
    direction.h \
    go.h \
    myslider.h \
    speed.h \
    stack.h \
    udp_thread.h \
        widget.h \
    headfile.h \
    deal_img.h

FORMS += \
        widget.ui

INCLUDEPATH += D:\opencv\opencv\build\include
D:\opencv\opencv\build\include\opencv2
LIBS += -L D:\OpenCVbuid\lib\libopencv_*.a \

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
