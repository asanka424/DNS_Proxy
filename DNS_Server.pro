#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T14:15:57
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = DNS_Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    dnsrequest.cpp \
    dnslistner.cpp \
    logger.cpp

HEADERS += \
    dnsrequest.h \
    dnslistner.h \
    logger.h
