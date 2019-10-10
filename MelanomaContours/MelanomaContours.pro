#-------------------------------------------------
#
# Project created by QtCreator 2019-10-10T21:21:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MelanomaContours
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14

INCLUDEPATH += open_cv/include

LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_calib3d320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_core320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_features2d320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_highgui320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_imgcodecs320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_photo320.dll
LIBS += $${_PRO_FILE_PWD_}/open_cv/bin/libopencv_imgproc320.dll


SOURCES += \
        main.cpp \
    Controllers/mainwindow.cpp \
    Abstract/singletoneclass.cpp \
    Models/appstorage.cpp \
    Managers/colorgenerator.cpp \
    Managers/managerslocator.cpp \
    Managers/mathmanager.cpp

HEADERS += \
    Controllers/mainwindow.h \
    Abstract/singletoneclass.h \
    Common/consts.h \
    Common/magic.h \
    Models/appstorage.h \
    Managers/colorgenerator.h \
    Managers/managerslocator.h \
    Managers/mathmanager.h \
    Common/opencv.h

FORMS += \
    Forms/mainwindow.ui
