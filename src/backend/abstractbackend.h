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
#ifndef ABSTRACTBACKEND_H
#define ABSTRACTBACKEND_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class AbstractBackend : public QObject {
  Q_OBJECT

public:
  explicit AbstractBackend(QNetworkAccessManager *manager,
                           QObject *parent = nullptr);
  ~AbstractBackend() = 0;

  Q_INVOKABLE virtual void getIncidents() = 0;

  Q_SIGNAL void requestError(const QString &errorMessage);

  // signals for the qml part
  Q_SIGNAL void getIncidentsResultAvailable(const QString &reply);

protected:
  QNetworkAccessManager *manager;

  QNetworkReply *executeGetRequest(const QUrl &url);
  void connectErrorSlot(QNetworkReply *reply);
};

#endif // ABSTRACTBACKEND_H
