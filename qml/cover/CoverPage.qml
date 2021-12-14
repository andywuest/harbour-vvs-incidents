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

import "../components"
import "../components/thirdparty"

CoverBackground {
    id: coverPage
    property bool networkError: false
    property bool loading : false
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
          incidentLines.text = qsTr("No incidents");
      } else {
          incidentLines.text = resultString
      }

      lastUpdate = new Date();

      networkError = false;
      loading = false;
    }

    function errorResultHandler(result) {
        Functions.log("[CoverPage] - result error : " + result);
        incidentUpdateNotification.show(result)
        label.text = qsTr("Error occured")
        networkError = true;
        loading = false;
    }

    function isIncidentPresent() {
        return (latestIncidents && latestIncidents.currentIncidents && latestIncidents.currentIncidents.length > 0);
    }

    function reloadAllIncidents() {
        Functions.log("[CoverPage] - reloadAllIncidents");
        disconnectSlots();
        connectSlots();

        loading = true;

        Functions.getDataBackend(Constants.BACKEND_STUTTGART).getIncidents()
    }

    CoverLoadingColumn {
        id: coverLoadingColumn
        visible: coverPage.loading
    }

    Column {
        id: incidentsColumn
        width: parent.width
        visible: !coverPage.loading

//        Column {
//            id: episodeTitleColumn

//            x: Theme.paddingMedium
//            y: Theme.paddingLarge
//            spacing: Theme.paddingSmall
//            width: parent.width - 2*Theme.paddingMedium

//            Item {
//                width: parent.width
//                height: incidentTitle.height

//                Label {
//                    id: incidentTitle
//                    text: "" // TODO
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    width: parent.width
//                        // Math.min(implicitWidth, parent.width)
//                    maximumLineCount:  6
//                    wrapMode: Text.WordWrap
//                    font.pixelSize: Theme.fontSizeSmall
//                    // lineHeightMode: Text.FixedHeight
//                    // lineHeight: Theme.itemSizeMedium / 2
//                    /*
//                    onLineLaidOut: {
//                        if (line.number == maximumLineCount - 1) {
//                            line.width = parent.width + 1000
//                        }
//                    }
//                    */
//                }


//                OpacityRampEffect {
//                    offset: 0.66
//                    sourceItem: incidentTitle
//                    enabled: incidentTitle.implicitWidth > Math.ceil(incidentTitle.width)
//                }
//            }
//        }

        Label {
            id: incidentLinesTitle
            y: Theme.paddingLarge
            x: Theme.paddingMedium
            width: parent.width - 2 * Theme.paddingMedium

            text: "Incidents for the lines:"
            anchors.horizontalCenter: parent.horizontalCenter
            maximumLineCount:  2
            wrapMode: Text.WordWrap
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            id: incidentLines
            x: Theme.paddingMedium
            y: Theme.paddingLarge
            width: parent.width - 2 * Theme.paddingMedium

            text: ""
            anchors.topMargin: Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            maximumLineCount:  8
            wrapMode: Text.WordWrap
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }





//    Label {
//        id: label
//        anchors.centerIn: parent
//        text: ""
//        textFormat: Text.RichText
//        wrapMode: Text.Wrap
//        visible: !coverPage.loading
//        opacity: coverPage.loading ? 0 : 1
//    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: {
                reloadAllIncidents();
            }
        }
    }

    Component.onCompleted: {
        connectSlots();
        reloadAllIncidents();
    }

    Component.onDestruction: {
        Functions.log("disconnecting signal");
        disconnectSlots();
    }

    OpacityRampEffect {
        sourceItem: incidentsColumn
        direction: OpacityRamp.TopToBottom
        offset: 0.6
        slope: 3.75
    }

}
