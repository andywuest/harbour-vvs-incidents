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
#include "backendstuttgarttests.h"
#include <QtTest/QtTest>

void BackendStuttgartTests::init() {
  backendStuttgart = new BackendStuttgart(nullptr, nullptr);
}

void BackendStuttgartTests::testBackendStuttgartProcessSearchResult() {
  QByteArray data = readFileData("incidents_ubahn.json");
  if (data.isEmpty()) {
    QString msg = "Testfile incidents_ubahn.json not found!";
    QFAIL(msg.toLocal8Bit().data());
  }

  QString parsedResult = backendStuttgart->processSearchResult(data);

  QJsonDocument jsonDocument = QJsonDocument::fromJson(parsedResult.toUtf8());
  QCOMPARE(jsonDocument.isObject(), true);

  QJsonArray resultArray = jsonDocument["currentIncidents"].toArray();
  QCOMPARE(resultArray.size(), 1);

  QJsonObject incidentsEntry = resultArray.at(0).toObject();
  QCOMPARE(incidentsEntry["title"], "Linien U19, U2");
  QCOMPARE(incidentsEntry["subtitle"],
           "Bad Cannstatt Wilhelmsplatz - Neugereut: Streckenunterbrechung "
           "wegen eines Verkehrsunfalls");

  //    qDebug() << parsedResult;
}

QByteArray BackendStuttgartTests::readFileData(const QString &fileName) {
  QFile f("testdata/" + fileName);
  if (!f.open(QFile::ReadOnly | QFile::Text)) {
    QString msg = "Testfile " + fileName + " not found!";
    return QByteArray();
  }

  QTextStream in(&f);
  return in.readAll().toUtf8();
}
