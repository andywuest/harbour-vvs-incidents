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
  //QNetworkReply *reply = executeGetRequest(QUrl("https://api.jsonbin.io/b/619944150ddbee6f8b0f4e93")); // bus viele
  //QNetworkReply *reply = executeGetRequest(QUrl("https://api.jsonbin.io/b/619e946462ed886f91542d82")); // einzeln




  // https://jsonbin.io/619944150ddbee6f8b0f4e93
  QNetworkReply *reply = executeGetRequest(QUrl("https://www3.vvs.de/mngvvs/XML_ADDINFO_REQUEST?AIXMLReduction=removeSourceSystem&SpEncId=0&coordOutputFormat=EPSG:4326&filterMessageSubtype=disruption:lines&filterMessageSubtype=disruption:stops&filterPublicationStatus=current&filterShowLineList=0&filterShowPlaceList=0&filterShowStopList=0&outputFormat=rapidJSON&serverInfo=1&version=10.2.10.139"));

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this, SLOT(handleGetIncidentsFinished()));
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
      QString creationTimestamp = timestamps["creation"].toString();

      qDebug()<< "title: " << currentObject["title"];
      qDebug()<< "timestamp: " << creationTimestamp;
      qDebug()<< "timestamp (con): " << convertTimestampToLocalTimestamp(creationTimestamp, QTimeZone::systemTimeZone());
      qDebug()<< "timestamp (format): " << convertToDatabaseDateTimeFormat(convertTimestampToLocalTimestamp(creationTimestamp, QTimeZone::systemTimeZone()));


      currentObject.insert("_timestampFormatted", convertToDatabaseDateTimeFormat(convertTimestampToLocalTimestamp(creationTimestamp, QTimeZone::systemTimeZone())));

      resultArray.push_back(currentObject);
    }

    //        foreach (const QJsonValue &newsEntry, newsItemArray) {
    //            QJsonObject newsObject = newsEntry.toObject();
    //            QString headline = newsObject["headline"].toString();
    //            QString content = newsObject["content"].toString();
    //            QString source = newsObject["id"].toString();
    //            QString url = QString::null;                          // not
    //            supported QString dateTime =
    //            newsObject["newsDate"].toString(); // TODO parsen in richtiges
    //            datetime

    //            QJsonObject resultObject;

    //            resultObject.insert("headline", headline);
    //            resultObject.insert("content", filterContent(content));
    //            resultObject.insert("source", source);
    //            resultObject.insert("url", url);
    //            resultObject.insert("dateTime",
    //                                IngDibaUtils::convertTimestampToLocalTimestamp(dateTime,
    //                                QTimeZone::systemTimeZone())
    //                                    .toString());

    //            // TODO evtl. html tags filtern -  Link-Tags entfernen <a>

    //            resultArray.push_back(resultObject);
    //        }
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
QDateTime BackendStuttgart::convertTimestampToLocalTimestamp(const QString &utcDateTimeString, QTimeZone timeZone) {
    QDateTime dt = QDateTime::fromString(utcDateTimeString, Qt::ISODate);
    dt.setTimeZone(timeZone);
    qDebug() << "dt : " << dt << "using timezone : " << timeZone;
    QDateTime localDateTime = dt.toLocalTime();
    return localDateTime;
}

QString BackendStuttgart::convertToDatabaseDateTimeFormat(const QDateTime &time) {
    return time.toString("yyyy-MM-dd") + " " + time.toString("hh:mm:ss");
}
