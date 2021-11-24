/**
 * harbour-vvs-incidents - Sailfish OS Version
 * Copyright © 2021 Andreas Wüst (andreas.wuest.freelancer@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QtQml>

#include "incidents.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/harbour-vvs-incidents.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    // return SailfishApp::main(argc, argv);

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    QQmlContext *context = view.data()->rootContext();
    context->setContextProperty("applicationVersion", QString(VERSION_NUMBER));

    Incidents incidents;
    context->setContextProperty("incidents", &incidents);

    BackendStuttgart *backendStuttgart = incidents.getBackendStuttgart();
    context->setContextProperty("backendStuttgart", backendStuttgart);

    view->setSource(SailfishApp::pathTo("qml/harbour-vvs-incidents.qml"));
    view->show();
    return app->exec();
}
