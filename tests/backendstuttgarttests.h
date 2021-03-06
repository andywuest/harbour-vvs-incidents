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
#ifndef ING_DIBA_BACKEND_TEST_H
#define ING_DIBA_BACKEND_TEST_H

#include <QObject>

#include "src/backend/backendstuttgart.h"

class BackendStuttgartTests : public QObject {
  Q_OBJECT

private:
  BackendStuttgart *backendStuttgart;

protected:
  QByteArray readFileData(const QString &fileName);

private slots:
  void init();

  // ING-DIBA Security Backend
  //    void testIngDibaUtilsConvertTimestampToLocalTimestamp();
  //    void testIngDibaBackendIsValidSecurityCategory();
  void testBackendStuttgartProcessSearchResult();
};

#endif // ING_DIBA_BACKEND_TEST_H
