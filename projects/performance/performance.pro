QT += charts qml quick

CONFIG += c++1z
QMAKE_CXXFLAGS += -stdlib=libc++ -Wall
# QMAKE_CXXFLAGS += -stdlib=libc++ -fsanitize=address -Wall
QMAKE_LFLAGS += -lc++ -lc++abi
# QMAKE_LFLAGS += -lc++ -lc++abi -fsanitize=address


SOURCES += main.cpp \
    datapacket.cpp \
    datamonitor.cpp \
    garbagepoolallocator.cpp \
    ../../src/garbage_pool_participant.cpp \
    ../../src/garbage_pool.cpp \
    ../../src/atomic_statistics.cpp \
    ../../src/statistics.cpp \
    customdata.cpp \
    datamodel.cpp \
    plainallocator.cpp \
    referencecounter.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    datapacket.h \
    datamonitor.h \
    garbagepoolallocator.h \
    garbagepoolallocator.h \
    ../../src/gp.h \
    idatasource.h \
    ../../src/configuration.h \
    ../../src/garbage_pool_participant.h \
    ../../src/garbage_pool.h \
    ../../src/gp.h \
    ../../src/queued_item.h \
    ../../src/details/has_deallocate.h \
    ../../src/atomic_statistics.h \
    ../../src/statistics.h \
    customdata.h \
    testobjects.h \
    datamodel.h \
    plainallocator.h \
    referencecounter.h
