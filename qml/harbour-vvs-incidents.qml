/*
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
import QtQuick 2.0
import Sailfish.Silica 1.0

import "pages"
import "cover"

import "js/constants.js" as Constants
import "js/functions.js" as Functions

ApplicationWindow {
    id: app

    signal incidentDataChanged(var incidentData, string error, date lastUpdate)

    function getDataBackend(backendId) {
        if (Constants.BACKEND_STUTTGART === backendId) {
            Functions.log("[ApplicationWindow] - backend is : " + backendStuttgart);
            return backendStuttgart;
        }
    }

    function reloadAllIncidents() {
        Functions.log("[ApplicationWindow] - reloadAllIncidents");
        var backend = getDataBackend(Constants.BACKEND_STUTTGART);
        disconnectSlots(backend);
        connectSlots(backend);
        backend.getIncidents()
    }

    function connectSlots(backend) {
        Functions.log("[ApplicationWindow] - connect slot " + backend);
        backend.getIncidentsResultAvailable.connect(getIncidentsResultHandler);
        backend.requestError.connect(errorResultHandler);
    }

    function disconnectSlots(backend) {
        Functions.log("[ApplicationWindow] disconnect - slots");
        backend.getIncidentsResultAvailable.disconnect(getIncidentsResultHandler);
        backend.requestError.disconnect(errorResultHandler);
    }

    function getIncidentsResultHandler(result) {
      Functions.log("[ApplicationWindow] result : " + result);
      incidentDataChanged(JSON.parse(result.toString()), "", new Date());
    }

    function errorResultHandler(result) {
        Functions.log("[ApplicationWindow] - result error : " + result);
        incidentDataChanged({}, result, new Date());
    }

    Component {
        id: overviewPage
        OverviewPage {
        }
    }

    Component {
        id: coverPage
        CoverPage {
        }
    }

    initialPage: overviewPage
    cover: coverPage
    allowedOrientations: defaultAllowedOrientations
}
