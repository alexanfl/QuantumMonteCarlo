TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    system.cpp

LIBS += -llapack -lblas -larmadillo

HEADERS += \
    catch.h \
    system.h

