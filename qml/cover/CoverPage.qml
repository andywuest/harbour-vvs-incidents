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

import "../js/constants.js" as Constants
import "../js/functions.js" as Functions

import "../components/thirdparty"

CoverBackground {

    property bool networkError: false
    property bool loaded : false
    property date lastUpdate

    function connectSlots() {
        console.log("connect - slots");
        var backend = Functions.getDataBackend(Constants.BACKEND_STUTTGART);
        console.log("[CoverPage] - connect slot " + backend);
        backend.getIncidentsResultAvailable.connect(getIncidentsResultHandler);
        backend.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("disconnect - slots");
        var backend = Functions.getDataBackend(Constants.BACKEND_STUTTGART);
        backend.getIncidentsResultAvailable.disconnect(getIncidentsResultHandler);
        backend.requestError.disconnect(errorResultHandler);
    }

    function getIncidentsResultHandler(result) {
      Functions.log("result : " + result);

      latestIncidents = JSON.parse(result.toString())
      var currentIncidents = latestIncidents["currentIncidents"];
      var resultString = "";
      Functions.log("[CoverPage] - json result from market data backend was: " + result)
      for (var i = 0; i < currentIncidents.length; i++)   {
          var incident = currentIncidents[i];
          if (resultString !== "") {
              resultString += ", ";
          }
          resultString += Functions.getListOfAffectedLines(incident.affected)
      }

      Functions.log("[CoverPage] - affected lines : " + resultString);

      if (resultString === "") {
          label.text = qsTr("No incidents");
      } else {
          label.text = qsTr("Incidents for the following lines : %1").arg(resultString);
      }

      lastUpdate = new Date();

      networkError = false;
      loaded = true;
    }

    function errorResultHandler(result) {
        Functions.log("[CoverPage] - result error : " + result);
        incidentUpdateNotification.show(result)
        label.text = qsTr("Error occured")
        networkError = true;
        loaded = true;
    }

    function isIncidentPresent() {
        return (latestIncidents && latestIncidents.currentIncidents && latestIncidents.currentIncidents.length > 0);
    }

    function reloadAllIncidents() {
        Functions.log("[CoverPage] - reloadAllIncidents");
        disconnectSlots();
        connectSlots();

        loaded = false;

        Functions.getDataBackend(Constants.BACKEND_STUTTGART).getIncidents()
    }

    Label {
        id: label
        anchors.centerIn: parent
        text: ""
        textFormat: Text.RichText
        wrapMode: Text.Wrap
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: {
                reloadAllIncidents();
            }
        }
    }

    LoadingIndicator {
        id: incidentsLoadingIndicator
        visible: !loaded
        Behavior on opacity {
            NumberAnimation {
            }
        }
        opacity: loaded ? 0 : 1
        height: parent.height
        width: parent.width
    }

    Component.onCompleted: {
        connectSlots();
        reloadAllIncidents();
    }

    Component.onDestruction: {
        Functions.log("disconnecting signal");
        disconnectSlots();
    }

}
