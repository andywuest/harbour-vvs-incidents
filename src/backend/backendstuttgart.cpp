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
#include <QRegularExpression>
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
  // QNetworkReply *reply =
  // executeGetRequest(QUrl("https://api.jsonbin.io/b/619944150ddbee6f8b0f4e93"));
  // // bus viele
  // QNetworkReply *reply =
  // executeGetRequest(QUrl("https://api.jsonbin.io/b/619e946462ed886f91542d82"));
  // // einzeln
  //   QNetworkReply *reply =
  //   executeGetRequest(QUrl("https://api.jsonbin.io/b/61db37872362237a3a35140c"));
  //   // zacke

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
//                             .replace(QString("func("), QString())
//                             .replace(QString(");"), QString());

    qDebug() << "result : " << searchJson;

    QJsonArray resultArray;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(searchJson.toUtf8());
    if (jsonDocument.isObject()) {
      QJsonObject rootObject = jsonDocument.object();
      // QJsonObject stopFindeObject = rootObject["stopFinder"].toObject();
      QJsonArray locationsArray = rootObject["locations"].toArray();

      foreach (const QJsonValue &locationEntry, locationsArray) {
        QJsonObject location = locationEntry.toObject();

//        QJsonArray assignedStopsArray = location["assignedStops"].toArray();

//        foreach (const QJsonValue &assignedStopEntry, assignedStopsArray) {
//            QJsonObject assignedStop = assignedStopEntry.toObject();
            resultArray.push_back(location);
//        }
      }
    }

//    std::sort(resultArray.begin(), resultArray.end(),
//              [](const QJsonValue &a, const QJsonValue &b) {
//                int qualityA = a.toObject()["quality"].toString().toInt();
//                int qualityB = b.toObject()["quality"].toString().toInt();

//                if (qualityA == qualityB) {
//                  QString placeA =
//                      a.toObject()["ref"].toObject()["place"].toString();
//                  QString placeB =
//                      b.toObject()["ref"].toObject()["place"].toString();

//                  qDebug() << "place a : " << placeA;

//                  int compareResult =
//                      QString::compare(placeA, placeB, Qt::CaseInsensitive);
//                  return compareResult < 0;
//                }

//                return qualityA > qualityB;
//              }); p

    QJsonDocument resultDocument;
    QJsonObject resultObject;
    resultObject.insert("locations", resultArray);
    resultDocument.setObject(resultObject);

    QString dataToString(resultDocument.toJson());

    qDebug() << "result : " << dataToString;

    emit searchStationResultAvailable(dataToString);
  });
}

inline void swap(QJsonValueRef v1, QJsonValueRef v2) {
  QJsonValue tmp(v1);
  v1 = QJsonValue(v2);
  v2 = tmp;
}

void BackendStuttgart::getLinesForStation(const QString &stationId) {
  qDebug() << "BackendStuttgart::getLinesForStation";
  qDebug() << "stationId: " << stationId;

//  QUrl url = QUrl(LINES_FOR_STATION_URL);
//  QNetworkRequest request(url);

//  request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);

//  const QString postData = QString(LINES_FOR_STATION_POST_DATA).arg(stationId);

//  qDebug() << "URL: " << url;
//  qDebug() << "postData: " << postData;

//  QNetworkReply *reply = manager->post(request, postData.toUtf8());

  QNetworkReply *reply =
      executeGetRequest(QUrl(QString(LINES_FOR_STATION_URL).arg(stationId)));

  connect(reply, &QNetworkReply::finished, this, [this, reply]() {
    reply->deleteLater();
    qDebug() << "return code : "
             << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute)
                    .toString();    

//    QByteArray searchReply = reply->readAll();

    QByteArray searchReply = reply->readAll();
    QString searchJson = QString(searchReply);

    qDebug() << "result : " << searchJson;

    QJsonDocument resultDocument;

    QJsonArray resultArray;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(searchJson.toUtf8());

    if (jsonDocument.isObject()) {
      QJsonObject rootObject = jsonDocument.object();
      resultDocument.setObject(rootObject);
    }

    QString dataToString(resultDocument.toJson());
    emit getLinesForStationResultAvailable(dataToString);
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

// 1) Reduce to the <select id="linien"> ... </select> block using regex
QString BackendStuttgart::extractLinienSelectBlock(const QString &html) const {
  // Pattern that captures the inner HTML of the select
  QRegularExpression reSelect(
      "<select[^>]*id\\s*=\\s*\"linien\"[^>]*>([\\s\\S]*?)</select>");

  reSelect.setPatternOptions(QRegularExpression::CaseInsensitiveOption |
                             QRegularExpression::DotMatchesEverythingOption);

  QRegularExpressionMatch m = reSelect.match(html);
  if (!m.hasMatch())
    return QString();

  // Group 1 = content between opening and closing select
  return m.captured(1);
}

QString BackendStuttgart::extractSessionId(const QString &html) const {
    // Static regex for efficiency (Qt 5.6+)
    static const QRegularExpression re(
            R"(<input\s+type=\"hidden\"\s+name=\"sessionID\"\s+id=\"sessionID\"\s+value=\"([^\"]+)\"\s*/?>)",
            QRegularExpression::CaseInsensitiveOption
        );

        QRegularExpressionMatch match = re.match(html);
        if (match.hasMatch()) {
            return match.captured(1);  // Returns the value inside the quotes
        }
        return QString();  // Empty if not found
}

QJsonObject BackendStuttgart::parseLinienSelectToJson(const QString &html) {
  QJsonArray resultArray;

  qDebug() << "reponse : " << html;

  // Step 0: get session id
  // const QString sessionId = extractSessionId(html);
  // qDebug() << "sessionId : " << sessionId;

  // Step 1: keep only the inner HTML of <select id="linien">
  const QString selectInner = extractLinienSelectBlock(html);
  if (selectInner.isEmpty()) {
    QJsonObject root;
    root.insert("result", resultArray);
    return root;
  }

  // Step 2: regex over <option ...>text</option>
  QRegularExpression reOption(
      "<option[^>]*value\\s*=\\s*\"([^\"]*)\"[^>]*>(.*?)</option>");

  reOption.setPatternOptions(QRegularExpression::CaseInsensitiveOption |
                             QRegularExpression::DotMatchesEverythingOption);

  QRegularExpressionMatchIterator it = reOption.globalMatch(selectInner);
  while (it.hasNext()) {
    QRegularExpressionMatch match = it.next();
    QString value = match.captured(1).trimmed();
    QString name = match.captured(2).trimmed();

    QString line = name.mid(0, name.indexOf("-"));
    name = name.mid(name.indexOf("-") + 2, name.length());

    // Remove any nested tags from the visible text, if present
    name.remove(
        REGULAR_EXPRESSION_NESTED_TAGS /*QRegularExpression("<[^>]*>")*/);

    QJsonObject optionObj;
    optionObj.insert("type", line.mid(0, line.indexOf(" ")).trimmed());
    optionObj.insert(
        "lineName",
        line.mid(line.indexOf(" "), line.lastIndexOf("-")).trimmed());
    optionObj.insert(
        "info", name.mid(name.lastIndexOf("-") + 1, name.length()).trimmed());
    optionObj.insert("value", value);
    optionObj.insert("name", name.mid(0, name.lastIndexOf("-")));
    resultArray.append(optionObj);
  }

  QJsonObject root;
  root.insert("result", resultArray);
  // root.insert("sessionID", sessionId);
  return root;
}

const QRegularExpression
    BackendStuttgart::REGULAR_EXPRESSION_NESTED_TAGS("<[^>]*>");
