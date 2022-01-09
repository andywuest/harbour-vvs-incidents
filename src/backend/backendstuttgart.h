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
#ifndef BACKENDSTUTTGART_H
#define BACKENDSTUTTGART_H

#include "abstractbackend.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QTimeZone>

class BackendStuttgart : public AbstractBackend {
  Q_OBJECT
public:
  explicit BackendStuttgart(QNetworkAccessManager *manager,
                            QObject *parent = nullptr);
  ~BackendStuttgart() override;

  Q_INVOKABLE virtual void getIncidents() override;

private slots:
  void handleGetIncidentsFinished();

protected:
  QString processSearchResult(QByteArray searchReply);
  QDateTime convertTimestampToLocalTimestamp(const QString &utcDateTimeString,
                                             const QTimeZone &timeZone);
  QString convertToDateTimeFormat(const QDateTime &time);
  QString convertToDateFormat(const QDateTime &time);

#ifdef UNIT_TEST
  friend class BackendStuttgartTests; // to test non public methods
#endif
};

#endif // BACKENDSTUTTGART_H
