QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    checkuserdata.cpp \
    createorder.cpp \
    deleteaccount.cpp \
    editorders.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    showorder.cpp

HEADERS += \
    checkuserdata.h \
    createorder.h \
    deleteaccount.h \
    editorders.h \
    login.h \
    mainwindow.h \
    showorder.h

FORMS += \
    checkuserdata.ui \
    createorder.ui \
    deleteaccount.ui \
    editorders.ui \
    login.ui \
    mainwindow.ui \
    showorder.ui

TRANSLATIONS += \
    Warsztat_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
