# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-frax-firewatcher
QT += multimedQT += feedback
INCLUDEPATH += $$(MER_SSH_SHARED_TARGET)/$$(MER_SSH_TARGET_NAME)/usr/include/c++/4.8.3
QMAKE_CXXFLAGS += -std=c++0x

CONFIG += sailfishapp

SOURCES += src/harbour-frax-firewatcher.cpp \
    src/vedtimer.cpp \
    src/libiphb/libiphb.c

OTHER_FILES += qml/harbour-frax-firewatcher.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/spis_large.png \
    rpm/harbour-frax-firewatcher.changes.in \
    rpm/harbour-frax-firewatcher.spec \
    harbour-frax-firewatcher.desktop

HEADERS += \
    src/vedtimer.h \
    src/libiphb/libiphb.h \
    src/libiphb/iphb_internal.h \
    src/libiphb/messages.h

RESOURCES += \
    sound.qrc
