
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
    property bool errorOccured: false
    property bool loading: false

    function incidentDataChanged(result, error, date) {
        Functions.log(
                    "[CoverPage] - data has changed, error " + error + ", date : " + date)
        errorOccured = (error !== "")

        coverModel.clear()

        if (!errorOccured) {
            for (var i = 0; i < result.currentIncidents.length; i++) {
                coverModel.append(result.currentIncidents[i])
            }
        } else {
            // label.text = qsTr("Error occured")
        }
        loading = false
    }

    CoverLoadingColumn {
        id: coverLoadingColumn
        visible: loading
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: {
                coverPage.loading = true
                coverPage.errorOccured = false
                app.reloadAllIncidents()
            }
        }
    }

    SilicaListView {
        id: coverListView

        visible: !coverPage.loading
        Behavior on opacity {
            NumberAnimation {
            }
        }
        opacity: coverPage.loading ? 0 : 1

        anchors.fill: parent

        model: ListModel {
            id: coverModel
        }

        header: Text {
            id: labelTitle
            width: parent.width
            text: qsTr("Incidents")
            color: Theme.primaryColor
            font.bold: true
            font.pixelSize: Theme.fontSizeSmall
            textFormat: Text.StyledText
            horizontalAlignment: Text.AlignHCenter
        }

        delegate: ListItem {
            contentHeight: incidentColumn.height + Theme.paddingSmall

            // TODO custom - hier noch pruefen, was an margins noch machbar, sinnvoll ist
            Column {
                id: incidentColumn
                x: Theme.paddingLarge
                width: parent.width - 2 * Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter

                IconLabelRow {
                    id: iconLabelRow
                    lineType: Functions.resolveIconForLines(affected)
                    affectedLines: Functions.getListOfAffectedLines(affected)
                }
            }
        }
    }

    Component.onCompleted: {
        app.incidentDataChanged.connect(incidentDataChanged)
    }

    OpacityRampEffect {
        sourceItem: coverListView
        direction: OpacityRamp.TopToBottom
        offset: 0.6
        slope: 3.75
    }
}
