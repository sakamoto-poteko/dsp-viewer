QT += gui core widgets

TEMPLATE = app
TARGET = viewer

HEADERS += FFTCalculator.h \
           FFTScaleDraw.h \
           shared_Ipp_ptr.h \
           TransposedMatrixRasterData.h \
           wave.h \
           FFTSpectrumWidget.h
    
SOURCES += FFTCalculator.cpp \
           FFTScaleDraw.cpp \
           main.cpp \
           wave.cpp \
           FFTSpectrumWidget.cpp
           
INCLUDEPATH += D:\Projects\Tools\qwt-6.1.2\include
QMAKE_LIBDIR += D:\Projects\Tools\qwt-6.1.2\lib

