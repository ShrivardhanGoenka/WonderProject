QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    ipdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    wondercam.cpp

HEADERS += \
    camera.h \
    ipdialog.h \
    mainwindow.h \
    wondercam.h

FORMS += \
    ipdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../Dropbox/PC (2)/Downloads/test/opencv/build/x64/vc15/lib/' -lopencv_world455
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../Dropbox/PC (2)/Downloads/test/opencv/build/x64/vc15/lib/' -lopencv_world455d
else:unix: LIBS += -L$$PWD/'../../../Dropbox/PC (2)/Downloads/test/opencv/build/x64/vc15/lib/' -lopencv_world455

INCLUDEPATH += $$PWD/'../../../Dropbox/PC (2)/Downloads/test/opencv/build/include'
DEPENDPATH += $$PWD/'../../../Dropbox/PC (2)/Downloads/test/opencv/build/include'

unix|win32: LIBS += -L$$PWD/'../../../Dropbox/PC (2)/Downloads/IRS_ThermalCamera_SDK_winx86_x64_V1056/IRS_ThermalCamera_SDK_winx86_x64_V1056/SDK/x64/' -lIRSThermalCameraSDK

INCLUDEPATH += $$PWD/'../../../Dropbox/PC (2)/Downloads/IRS_ThermalCamera_SDK_winx86_x64_V1056/IRS_ThermalCamera_SDK_winx86_x64_V1056/SDK/include'
DEPENDPATH += $$PWD/'../../../Dropbox/PC (2)/Downloads/IRS_ThermalCamera_SDK_winx86_x64_V1056/IRS_ThermalCamera_SDK_winx86_x64_V1056/SDK/include'

RESOURCES += \
    mainwindow.qrc
