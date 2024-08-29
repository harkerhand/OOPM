# OOPM : Object-Oriented Project Management

TRANSLATIONS += langs/OOPM_zh_CN.ts \
                langs/OOPM_en_US.ts

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
    LoginWindow.cpp \
    MemberDetailsDialog.cpp \
    MembersDetailDialog.cpp \
    UserManagementWindow.cpp \
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
    LoginWindow.h \
    MainWindow.h \
    MemberDetailsDialog.h \
    MemberModel.h \
    MembersDetailDialog.h \
    SimpleTextDialog.h \
    UserManagementWindow.h \
    UserModel.h \
    Utils.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



RESOURCES += \
    resources.qrc
