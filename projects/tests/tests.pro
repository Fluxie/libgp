QT += core
QT += testlib
QT -= gui

CONFIG += c++1z

QMAKE_CXXFLAGS += -stdlib=libc++ -Wall
QMAKE_LFLAGS += -lc++ -lc++abi

TARGET = tests
CONFIG += console
CONFIG += no_batch
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../src/garbage_pool.cpp \
    ../../src/garbage_pool_participant.cpp \
    ../../tests/single_participant.cpp \
    ../../tests/multiple_participants.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    ../../src/garbage_pool.h \
    ../../src/garbage_pool_participant.h \
    ../../src/configuration.h \
    ../../tests/single_participant.h \
    ../../src/gp.h \
    ../../src/details/has_deallocate.h \
    ../../src/queued_item.h \
    ../../tests/details/d_wrapper.h \
    ../../testd.h \
    ../../tests/multiple_participants.h
