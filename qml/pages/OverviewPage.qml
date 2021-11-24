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
import QtQuick 2.2
import Sailfish.Silica 1.0

import "../js/constants.js" as Constants
import "../js/functions.js" as Functions

import "../components/thirdparty"

Page {
    id: page

    property bool loaded : false

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    function connectSlots() {
        console.log("connect - slots");
        var backend = Functions.getDataBackend(Constants.BACKEND_STUTTGART);
        console.log("[OverviewPage] - connect slot " + backend);
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

      var jsonResult = JSON.parse(result.toString())
      var currentIncidents = jsonResult["currentIncidents"];
      Functions.log("json result from market data backend was: " + result)
      for (var i = 0; i < currentIncidents.length; i++)   {
          var incident = currentIncidents[i];
          incidentsModel.append(incident);
          Functions.log("added incident " + incident.title);
      }

//          var loadedMarketData = Database.loadMarketDataBy('' + marketData.extRefId)
//          if (loadedMarketData) {
//              // copy id / typeId
//              marketData.id = loadedMarketData.id;
//              marketData.typeId = loadedMarketData.typeId;
//              // persist
//              Database.persistMarketdata(marketData)
//          }
//      }
//      reloadAllMarketData()


      loaded = true;
    }

    function errorResultHandler(result) {
        console.log("result error : " + result)
        // marketDataUpdateProblemNotification.show(result)
        loaded = true;
    }

    function reloadAllIncidents() {
        Functions.log("[OverviewPage] - reloadAllIncidents");
        disconnectSlots();
        connectSlots();

        incidentsModel.clear()

        loaded = false;

        Functions.getDataBackend(Constants.BACKEND_STUTTGART).getIncidents()
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        id: pageFlickable
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Show Page 2")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
            MenuItem {
                text: qsTr("Reload Incidents")
                onClicked: reloadAllIncidents();
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column. The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                id: incidentsHeader
                //: OverviewPage page header
                title: qsTr("Incidents")
            }

            SilicaListView {
                id: incidentsListView

                height: pageFlickable.height - incidentsHeader.height - Theme.paddingMedium
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right

                clip: true

                model: ListModel {
                    id: incidentsModel
                }

                delegate: ListItem {
                    contentHeight: incidentItem.height + (2 * Theme.paddingMedium)
                    contentWidth: parent.width

                    onClicked: {
                        var selectedIncident = incidentsListView.model.get(index);
                        pageStack.push(Qt.resolvedUrl("../pages/DetailsPage.qml"), { incident: selectedIncident }) // TODO page url
                    }

                    Item {
                        id: incidentItem
                        width: parent.width
                        height: incidentRow.height + incidentSeparator.height
                        y: Theme.paddingMedium

                        Row {
                            id: incidentRow
                            width: parent.width - (2 * Theme.horizontalPageMargin)
                            spacing: Theme.paddingMedium
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter

                            // TODO custom - hier noch pruefen, was an margins noch machbar, sinnvoll ist
                            Column {
                                id: stockQuoteColumn
                                width: parent.width // - (2 * Theme.horizontalPageMargin)
                                // x: Theme.horizontalPageMargin
                                height: firstRow.height + changeValuesRow.height + secondRow.height
                                /* + secondRow.height*/
                                        //+ changeValuesRow.height
                                        //+ (watchlistSettings.showPerformanceRow ? performanceRow.height : 0)

                                anchors.verticalCenter: parent.verticalCenter

                                Row {
                                    id: firstRow
                                    width: parent.width
                                    height: Theme.fontSizeSmall + Theme.paddingMedium

                                    Label {
                                        id: stockQuoteName
                                        width: parent.width * 8 / 10
                                        height: parent.height
                                        text: title
                                        truncationMode: TruncationMode.Fade// TODO check for very long texts
                                        // elide: Text.ElideRight
                                        color: Theme.primaryColor
                                        font.pixelSize: Theme.fontSizeSmall
                                        font.bold: true
                                        horizontalAlignment: Text.AlignLeft
                                    }

                                    Text {
                                        id: stockQuoteChange
                                        width: parent.width * 2 / 10
                                        height: parent.height
                                        text: "value1"
                                            //Functions.renderPrice(price, currencySymbol);
                                        color: Theme.highlightColor
                                        font.pixelSize: Theme.fontSizeSmall
                                        font.bold: true
                                        horizontalAlignment: Text.AlignRight
                                    }
                                }

                                Row {
                                    id: secondRow
                                    width: parent.width
                                    height: Theme.fontSizeSmall + Theme.paddingMedium

                                    Image {
                                       id: rowIcon
                                       source: "../icons/" + "vvs_" + Functions.resolveIconForLines(affected) + ".svg"
                                       height: secondRow.height
                                       width: secondRow.height
                                       anchors.verticalCenter: parent.verticalCenter
                                    }

                                    Label {
                                        id: secondStockQuoteName
                                        // width: parent.width * 8 / 10
                                        height: parent.height
                                        width: secondRow.width - rowIcon.width
                                        text: Functions.getListOfAffectedLines(affected)
                                        truncationMode: TruncationMode.Fade// TODO check for very long texts
                                        // elide: Text.ElideRight
                                        color: Theme.primaryColor
                                        font.pixelSize: Theme.fontSizeSmall
                                        font.bold: true
                                        horizontalAlignment: Text.AlignLeft
                                    }

//                                    Text {
//                                        id: secondStockQuoteChange
//                                        // width: parent.width * 2 / 10
//                                        height: parent.height
//                                        text: "value1"
//                                            //Functions.renderPrice(price, currencySymbol);
//                                        color: Theme.highlightColor
//                                        font.pixelSize: Theme.fontSizeSmall
//                                        font.bold: true
//                                        horizontalAlignment: Text.AlignRight
//                                    }
                                }


                                Row {
                                    id: changeValuesRow
                                    width: parent.width
                                    height: Theme.fontSizeExtraSmall + Theme.paddingSmall

                                    Label {
                                        width: parent.width// / 2
                                        height: parent.height
                                        text: subtitle
                                            // Functions.determineQuoteDate(quoteTimestamp)
                                        truncationMode: TruncationMode.Fade
                                        color: Theme.primaryColor
                                        font.pixelSize: Theme.fontSizeExtraSmall
                                        horizontalAlignment: Text.AlignLeft
                                    }

//                                    Text {
//                                        width: parent.width / 2
//                                        height: parent.height
//                                        text: "value2"
//                                            // Functions.renderChange(price, changeRelative, '%')
//                                        // color: Functions.determineChangeColor(changeRelative)
//                                        font.pixelSize: Theme.fontSizeExtraSmall
//                                        horizontalAlignment: Text.AlignRight
//                                    }
                                }

                            }

                        }

                        Separator {
                            id: incidentSeparator
                            anchors.top: incidentRow.bottom
                            anchors.topMargin: Theme.paddingMedium

                            width: parent.width
                            color: Theme.primaryColor
                            horizontalAlignment: Qt.AlignHCenter
                        }

                    }

                }

            }
        }
    }

    Component.onCompleted: {
        connectSlots();
        reloadAllIncidents();
//        loaded = true;
    }

    Component.onDestruction: {
//        Functions.log("disconnecting signal");
        disconnectSlots();
    }

    LoadingIndicator {
        id: stocksLoadingIndicator
        visible: !loaded
        Behavior on opacity {
            NumberAnimation {
            }
        }
        opacity: loaded ? 0 : 1
        height: parent.height
        width: parent.width
    }

}
