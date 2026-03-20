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
  QCOMPARE(incidentsEntry["_fromFormatted"], "02.11.2021");
  QCOMPARE(incidentsEntry["_toFormatted"], "02.11.2021");

  //   qDebug() << parsedResult;
}

void BackendStuttgartTests::testParseLinienSelectToJson_noSelectWithOptions() {
  QString html = "<div>no select here</div>";
  QJsonObject result = backendStuttgart->parseLinienSelectToJson(html);
  QCOMPARE(result["result"].toArray().size(), 0);
}

void BackendStuttgartTests::testParseLinienSelectToJson_withOptions() {
  QString html = R"(
        <div>
            <select id="linien">
                <option value="10">Linie 10</option>
                <option value="20">Linie 20 nach Stadtmitte</option>
            </select>
        </div>
    )";

  QJsonObject result = backendStuttgart->parseLinienSelectToJson(html);
  QJsonArray array = result["result"].toArray();

  QCOMPARE(array.size(), 2);

  // First option
  QJsonObject opt1 = array[0].toObject();
  QCOMPARE(opt1["value"].toString(), QString("10"));
  QCOMPARE(opt1["name"].toString(), QString("Linie 10"));

  // Second option
  QJsonObject opt2 = array[1].toObject();
  QCOMPARE(opt2["value"].toString(), QString("20"));
  QCOMPARE(opt2["name"].toString(), QString("Linie 20 nach Stadtmitte"));
}

QByteArray BackendStuttgartTests::readFileData(const QString &fileName) {
  QFile f("../cpp/data/" + fileName);
  if (!f.open(QFile::ReadOnly | QFile::Text)) {
    qDebug() << "file not found : " << QFileInfo(f).absoluteFilePath();
    QString msg = "Testfile " + fileName + " not found!";
    return QByteArray();
  }

  QTextStream in(&f);
  return in.readAll().toUtf8();
}
