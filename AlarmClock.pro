QT       += core gui multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = alarmclock
TEMPLATE = app
QMAKE_LFLAGS += -no-pie

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/AboutDlg.cpp \
        src/AlarmClock.cpp \
        src/AlertTimeDlg.cpp \
        src/NowTimeDlg.cpp \
        src/SettingDlg.cpp


HEADERS += \
        src/AboutDlg.h \
        src/AlarmClock.h \
        src/AlertTimeDlg.h \
        src/NowTimeDlg.h \
        src/resource.h \
        src/SettingDlg.h

FORMS += \
        src/AboutDlg.ui \
        src/AlarmClock.ui \
        src/AlertTimeDlg.ui \
        src/NowTimeDlg.ui \
        src/SettingDlg.ui

RESOURCES += \
        src/AlarmClock.qrc

unix:!android {
    target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target
    DESKTOP.files += \
            alarmclock.desktop
    DESKTOP.path += \
            "/usr/share/applications/"
    IMAGES.files += \
            src/Resources/alarmclock.png
    IMAGES.path += \
            "/usr/share/pixmaps/"
    INSTALLS += DESKTOP IMAGES
}
