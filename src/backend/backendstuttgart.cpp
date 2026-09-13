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
#include "backendstuttgart.h"
#include "../constants.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

BackendStuttgart::BackendStuttgart(QNetworkAccessManager *manager,
                                   QObject *parent)
    : AbstractBackend(manager, parent) {
  qDebug() << "Initializing Stuttgart Backend...";
}

BackendStuttgart::~BackendStuttgart() {
  qDebug() << "Shutting down Stuttgart Backend...";
}

void BackendStuttgart::getIncidents() {
  qDebug() << "BackendStuttgart::searchName";

  QNetworkReply *reply = executeGetRequest(QUrl(INCIDENTS_VVS_URL));

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this, SLOT(handleGetIncidentsFinished()));
}

void BackendStuttgart::searchStation(const QString &searchString) {
  qDebug() << "BackendStuttgart::searchStation";

  QNetworkReply *reply =
      executeGetRequest(QUrl(QString(STATIONS_VVS_URL).arg(searchString)));

  connectErrorSlot(reply);

  connect(reply, &QNetworkReply::finished, this, [this, reply]() {
    reply->deleteLater();
    qDebug() << "return code : "
             << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute)
                    .toString();

    QByteArray searchReply = reply->readAll();
    QString searchJson = QString(searchReply);

    qDebug() << "result : " << searchJson;

    QJsonArray resultArray;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(searchJson.toUtf8());
    if (jsonDocument.isObject()) {
      QJsonObject rootObject = jsonDocument.object();
      // QJsonObject stopFindeObject = rootObject["stopFinder"].toObject();
      QJsonArray locationsArray = rootObject["locations"].toArray();

      foreach (const QJsonValue &locationEntry, locationsArray) {
        QJsonObject location = locationEntry.toObject();
        resultArray.push_back(location);
      }
    }

    QJsonDocument resultDocument;
    QJsonObject resultObject;
    resultObject.insert("locations", resultArray);
    resultDocument.setObject(resultObject);

    QString dataToString(resultDocument.toJson());

    qDebug() << "result : " << dataToString;

    emit searchStationResultAvailable(dataToString);
  });
}

void BackendStuttgart::getLinesForStation(const QString &stationId) {
  qDebug() << "BackendStuttgart::getLinesForStation";
  qDebug() << "stationId: " << stationId;

  QNetworkReply *reply =
      executeGetRequest(QUrl(QString(LINES_FOR_STATION_URL).arg(stationId)));

  connect(reply, &QNetworkReply::finished, this, [this, reply]() {
    reply->deleteLater();
    qDebug() << "return code : "
             << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute)
                    .toString();

    QByteArray searchReply = reply->readAll();
    QString searchJson = QString(searchReply);

    qDebug() << "result : " << searchJson;

    QJsonDocument resultDocument;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(searchJson.toUtf8());

    if (jsonDocument.isObject()) {
      QJsonObject rootObject = jsonDocument.object();
      resultDocument.setObject(rootObject);
    }

    QString dataToString(resultDocument.toJson());
    emit getLinesForStationResultAvailable(dataToString);
  });
}

void BackendStuttgart::getStationPlan(const QString &stationId,
                                      const QString &stationLineId) {
  qDebug() << "BackendStuttgart::getStationPlan";
  qDebug() << "stationId: " << stationId;
  qDebug() << "stationLineId: " << stationLineId;

  QNetworkReply *reply = executeGetRequest(QUrl(
      QString(STATION_LINE_PLAN_JSON_URL)
          .arg(QString(stationLineId).replace(" ", "+", Qt::CaseInsensitive),
               QString(stationId).replace(" ", "+", Qt::CaseInsensitive))));

  connect(reply, &QNetworkReply::finished, this, [this, reply]() {
    reply->deleteLater();
    qDebug() << "return code : "
             << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute)
                    .toString();

    QByteArray searchReply = reply->readAll();
    QString searchJson = QString(searchReply);

    qDebug() << "server response: " << searchJson;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(searchJson.toUtf8());

    if (jsonDocument.isObject()) {
      QJsonObject downloadsObject =
          jsonDocument.object()["download"].toObject();
      QString pdfUrlSuffix = downloadsObject["url"].toString();
      qDebug() << "pdfUrlSuffix : " << pdfUrlSuffix;
      emit getStationPlanAvailable(QString(DOWNLOAD_URL).arg(pdfUrlSuffix));
    } else {
      emit getStationPlanAvailable("{}");
    }
  });
}

void BackendStuttgart::handleGetIncidentsFinished() {
  qDebug() << "BackendStuttgart::handleGetIncidentsFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  QByteArray searchReply = reply->readAll();
  QJsonDocument jsonDocument = QJsonDocument::fromJson(searchReply);
  if (jsonDocument.isObject()) {
    emit getIncidentsResultAvailable(processSearchResult(searchReply));
  } else {
    qDebug() << "not a json object !";
  }
}

QString BackendStuttgart::processSearchResult(QByteArray searchReply) {
  QJsonArray resultArray;
  QJsonDocument jsonDocument = QJsonDocument::fromJson(searchReply);
  if (jsonDocument.isObject()) {
    QJsonObject rootObject = jsonDocument.object();
    QJsonObject infosObject = rootObject["infos"].toObject();
    QJsonArray currentArray = infosObject["current"].toArray();

    foreach (const QJsonValue &currentEntry, currentArray) {
      QJsonObject currentObject = currentEntry.toObject();
      QJsonObject timestamps = currentObject["timestamps"].toObject();
      QJsonObject availability = timestamps["availability"].toObject();

      QString creationTimestamp = timestamps["creation"].toString();
      QString fromTimestamp = availability["from"].toString();
      QString toTimestamp = availability["to"].toString();

      qDebug() << "title: " << currentObject["title"];
      qDebug() << "timestamp: " << creationTimestamp;
      qDebug() << "timestamp (con): "
               << convertTimestampToLocalTimestamp(creationTimestamp,
                                                   QTimeZone::systemTimeZone());
      qDebug() << "timestamp (format): "
               << convertToDateTimeFormat(convertTimestampToLocalTimestamp(
                      creationTimestamp, QTimeZone::systemTimeZone()));
      qDebug() << "from (format): "
               << convertToDateTimeFormat(convertTimestampToLocalTimestamp(
                      fromTimestamp, QTimeZone::systemTimeZone()));
      qDebug() << "to (format): "
               << convertToDateTimeFormat(convertTimestampToLocalTimestamp(
                      toTimestamp, QTimeZone::systemTimeZone()));

      const QTimeZone timezone = QTimeZone::systemTimeZone();

      currentObject.insert(
          "_timestampFormatted",
          convertToDateTimeFormat(
              convertTimestampToLocalTimestamp(creationTimestamp, timezone)));
      currentObject.insert("_fromFormatted",
                           convertToDateFormat(convertTimestampToLocalTimestamp(
                               fromTimestamp, timezone)));
      currentObject.insert("_toFormatted",
                           convertToDateFormat(convertTimestampToLocalTimestamp(
                               toTimestamp, timezone)));

      resultArray.push_back(currentObject);
    }
  }

  // response objects
  QJsonObject resultObject;
  resultObject.insert("currentIncidents", resultArray);

  QJsonDocument resultDocument;
  resultDocument.setObject(resultObject);

  QString dataToString(resultDocument.toJson());

  return dataToString;
}

// TODO copied from watchlist
QDateTime BackendStuttgart::convertTimestampToLocalTimestamp(
    const QString &utcDateTimeString, const QTimeZone &timeZone) {
  QDateTime dt = QDateTime::fromString(utcDateTimeString, Qt::ISODate);
  dt.setTimeZone(timeZone);
  qDebug() << "dt : " << dt << "using timezone : " << timeZone;
  QDateTime localDateTime = dt.toLocalTime();
  return localDateTime;
}

QString BackendStuttgart::convertToDateTimeFormat(const QDateTime &time) {
  return time.toString("dd.MM.yyyy") + " " + time.toString("hh:mm:ss");
}

QString BackendStuttgart::convertToDateFormat(const QDateTime &dateTime) {
  // if date is too far in the future -> return blank string
  if (dateTime.date().year() > 2100) {
    return QString("");
  }
  return dateTime.toString("dd.MM.yyyy");
}
