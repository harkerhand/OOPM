# OOPM : Object-Oriented Project Management



QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ClassInfo.cpp \
    ClassMember.cpp \
    CreateClassDialog.cpp \
    CreateMemberDialog.cpp \
    MemberDetailsDialog.cpp \
    MembersDetailDialog.cpp \
    Utils.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    ClassInfo.h \
    ClassMember.h \
    ClassModel.h \
    CreateClassDialog.h \
    CreateMemberDialog.h \
    Enums.h \
    MainWindow.h \
    MemberDetailsDialog.h \
    MemberModel.h \
    MembersDetailDialog.h \
    SimpleTextDialog.h \
    Utils.h

TRANSLATIONS += \
    OOPM_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    darkstyle.qss
