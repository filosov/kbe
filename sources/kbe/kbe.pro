# -------------------------------------------------
# Project created by QtCreator 2010-03-10T23:38:40
# -------------------------------------------------

DESTDIR = ../bin

QT      += xml sql network

TEMPLATE = app

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    extendedtabwidget.cpp \
    config.cpp \
    pluginmanager.cpp \
    guidedialog.cpp \
    newfiledialog.cpp \
    projectmanagerview.cpp \
    projectmanagermodel.cpp \
    projectmanagerdockwidget.cpp \
    preference/preferencedialog.cpp \
    outputwindow.cpp \
    preference/buildsettingsfactory.cpp \
    preference/buildsettingswidget.cpp \
    vcs/vcsmanager.cpp \
    rest-client/baserestclient.cpp \
    sql/querymanager.cpp \
    rest-client/youtrackrestclient.cpp \
    preference/trackersettingsfactory.cpp \
    preference/trackersettingswidget.cpp \
    vcs/vcsinterface.cpp \
    taskmanager.cpp \
    taskwindow.cpp \
    projectbuilder.cpp \
    application.cpp \
    taskview.cpp

HEADERS += version.h \
    platform.h \
    mainwindow.h \
    extendedtabwidget.h \
    config.h \
    interfaces/plugininterface.h \
    pluginmanager.h \
    interfaces/editorinterface.h \
    guidedialog.h \
    newfiledialog.h \
    projectmanagerview.h \
    projectmanagermodel.h \
    projectmanagerdockwidget.h \
    preference/preferencedialog.h \
    preference/abstractsettingswidgetfactory.h \
    outputwindow.h \
    preference/buildsettingsfactory.h \
    preference/buildsettingswidget.h \
    vcs/vcsinterface.hpp \
    vcs/vcsauthinterface.hpp \
    vcs/vcstransactioninterface.hpp \
    vcs/vcspropertyinterface.hpp \
    vcs/vcsfilestateinterface.hpp \
    vcs/vcsmanager.hpp \
    vcs/VCSBranchInterface.hpp \
    rest-client/baserestclient.hpp \
    rest-client/issue.hpp \
    rest-client/milestone.hpp \
    rest-client/project.hpp \
    sql/querymanager.hpp \
    rest-client/youtrackrestclient.hpp \
    preference/trackersettingsfactory.hpp \
    preference/trackersettingswidget.hpp \
    preference/abstractsettingswidget.hpp \
    rest-client/restitem.hpp \
    taskmanager.hpp \
    taskwindow.h \
    projectbuilder.hpp \
    application.hpp \
    taskview.hpp

INCLUDEPATH += preference

FORMS += mainwindow.ui

RESOURCES     = kbe.qrc

TRANSLATIONS += media/translations/lang_en_EN.ts \
                media/translations/lang_ru_RU.ts

lrelease.input         = TRANSLATIONS
lrelease.output        = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_IN_BASE}.qm
lrelease.commands      = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_IN_BASE}.qm
lrelease.CONFIG       += no_link target_predeps
QMAKE_EXTRA_COMPILERS += lrelease

RC_FILE = kbe.rc
#Debug:DEFINES += _DEBUG

OBJECTS_DIR = obj
MOC_DIR = moc

OTHER_FILES += \
    media/stylesheets/projectmanager.qss
