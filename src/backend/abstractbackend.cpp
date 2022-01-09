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
#include "abstractbackend.h"
#include "../constants.h"

AbstractBackend::AbstractBackend(QNetworkAccessManager *manager,
                                 QObject *parent)
    : QObject(parent) {
  qDebug() << "Initializing Backend...";
  this->manager = manager;
}

AbstractBackend::~AbstractBackend() {
  qDebug() << "Shutting down AbstractBackend...";
}

QNetworkReply *AbstractBackend::executeGetRequest(const QUrl &url) {
  qDebug() << "AbstractBackend::executeGetRequest " << url;
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);

  return manager->get(request);
}

void AbstractBackend::connectErrorSlot(QNetworkReply *reply) {
  // connect the error and also emit the error signal via a lambda expression
  connect(reply,
          static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(
              &QNetworkReply::error),
          [=](QNetworkReply::NetworkError error) {
            reply->deleteLater();
            qWarning() << "AbstractBackend::connectErrorSlot:"
                       << static_cast<int>(error) << reply->errorString()
                       << reply->readAll();
            emit requestError(
                "Return code: " + QString::number(static_cast<int>(error)) +
                " - " + reply->errorString());
          });
}
