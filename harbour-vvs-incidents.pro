# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-vvs-incidents

CONFIG += sailfishapp

SOURCES += src/harbour-vvs-incidents.cpp \
    src/incidents.cpp

HEADERS += \
    src/incidents.h

DEFINES += VERSION_NUMBER=\\\"$$(VERSION_NUMBER)\\\"

DISTFILES += qml/harbour-vvs-incidents.qml \
    qml/components/IconLabelRow.qml \
    qml/components/thirdparty/LabelText.qml \
    qml/components/thirdparty/LoadingIndicator.qml \
    qml/components/thirdparty/AppNotification.qml \
    qml/components/thirdparty/AppNotificationItem.qml \
    qml/cover/CoverPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/DetailsPage.qml \
    qml/pages/OverviewPage.qml \
    qml/js/functions.js \
    qml/js/constants.js \
    qml/icons/vvs_bus.svg \
    qml/icons/vvs_sbahn.svg \
    qml/icons/vvs_ubahn.svg \
    qml/icons/vvs_rbahn.svg \
    qml/icons/vvs_zacke.svg \
    rpm/harbour-vvs-incidents.changes.in \
    rpm/harbour-vvs-incidents.changes.run.in \
    rpm/harbour-vvs-incidents.spec \
    rpm/harbour-vvs-incidents.yaml \
    translations/*.ts \
    harbour-vvs-incidents.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-vvs-incidents-de.ts \
    translations/harbour-vvs-incidents-en.ts

include(harbour-vvs-incidents.pri)
