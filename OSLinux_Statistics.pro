QT       += core gui sql charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
RESOURCES += resources.qrc

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/buttons_clicked.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/additional_functions.cpp \
    src/processes_funcs.cpp \
    src/reading_from_file.cpp \
    src/parse_proc_dir.cpp\
    qcustomplot.cpp \
    src/right_click_slots.cpp \
    src/update_pages.cpp \
    src/work_with_graphs.cpp


HEADERS += \
    headers/mainwindow.h \
    headers/additional_functions.h \
    headers/reading_from_file.h \
    headers/parse_proc_dir.h \
    headers/structures.h \
    qcustomplot.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
