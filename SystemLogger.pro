#-------------------------------------------------
#
# Project created by QtCreator 2014-10-20T20:39:56
#
#-------------------------------------------------

QT       += core gui printsupport

QMAKE_CXXFLAGS += -fpermissive -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network xml

TARGET = SystemLogger
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    startpage.cpp \
    rss/rssfeed.cpp \
    ui/rssentry.cpp \
    ui/projectentry.cpp \
    projectview.cpp \
    project.cpp \
    ui/newprojectwizzard.cpp \
    3rd_party/qcustomplot/qcustomplot.cpp \
    ui/entrydialog.cpp \
    ui/dategraph.cpp \
    ui/trackabledataeditordialog.cpp \
    ui/trackabledatachecklist.cpp \
    ui/datadescriptoreditor.cpp \
    ui/datagraph.cpp \
    ui/dataentrydialog.cpp \
    ui/plottabledatachecklist.cpp \
    ui/menu.cpp

HEADERS  += mainwindow.h \
    settings.h \
    startpage.h \
    rss/rssfeed.h \
    rss/rsschannel.h \
    ui/rssentry.h \
    ui/projectentry.h \
    projectview.h \
    project.h \
    ui/newprojectwizzard.h \
    3rd_party/qcustomplot/qcustomplot.h \
    ui/entrydialog.h \
    abstractentry.h \
    dataentry.h \
    ui/dategraph.h \
    ui/trackabledataeditordialog.h \
    ui/trackabledatachecklist.h \
    ui/datadescriptoreditor.h \
    ui/layoutdata.h \
    ui/datagraph.h \
    ui/dataentrydialog.h \
    ui/plottabledatachecklist.h \
    ui/menu.h

FORMS    += mainwindow.ui \
    startpage.ui \
    ui/rssentry.ui \
    ui/projectentry.ui \
    projectview.ui \
    ui/newprojectwizzard.ui \
    ui/entrydialog.ui \
    ui/dategraph.ui \
    ui/trackabledataeditordialog.ui \
    ui/datadescriptoreditor.ui \
    ui/dataentrydialog.ui

OTHER_FILES += \
    ui/ 
