
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlobDetection
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += opencv4

LIBS += -ltesseract -llept

INCLUDEPATH += /usr/include/leptonica
INCLUDEPATH += /usr/include/tesseract

CONFIG += c++11

SOURCES += \
        bgblobdetector.cpp \
        centriaxmlparser.cpp \
        main.cpp \
        mainwindow.cpp
HEADERS += \
        bgblobdetector.h \
        centriaxmlparser.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    configuration.xml
