import QtQuick 2.2
import QtQuick.LocalStorage 2.0
import Sailfish.Silica 1.0

import "../js/constants.js" as Constants
import "../js/functions.js" as Functions

import "../components"
import "../components/thirdparty"

Page {
    id: stationSearchPage

    property string stationName: ""
    property string stationId: ""

    allowedOrientations: Orientation.All

    function connectSlots() {
        Functions.log("StationSearchPage - connecting - slots")
        var dataBackend = getDataBackend(Constants.BACKEND_STUTTGART);
        dataBackend.searchStationResultAvailable.connect(searchStationResultHandler);
        dataBackend.getLinesForStationResultAvailable.connect(getLinesForStationResultHandler);
        dataBackend.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        Functions.log("StationSearchPage - disconnecting - slots")
        var dataBackend = getDataBackend(Constants.BACKEND_STUTTGART);
        dataBackend.searchStationResultAvailable.disconnect(searchStationResultHandler);
        dataBackend.getLinesForStationResultAvailable.disconnect(getLinesForStationResultHandler);
        dataBackend.requestError.disconnect(errorResultHandler);
    }

    function searchStationResultHandler(result) {
      var jsonResult = JSON.parse(result.toString())
      Functions.log("json result from backend was: " + result)

      for (var i = 0; i < jsonResult.locations.length; i++) {
          if (jsonResult.locations[i] && jsonResult.locations[i].type === "stop") {
            if (!jsonResult.locations[i].assignedStops) {
                searchResultListModel.append(jsonResult.locations[i]);
            } else {
                for (var j = 0; j < jsonResult.locations[i].assignedStops.length; j++) {
                  searchResultListModel.append(jsonResult.locations[i].assignedStops[j]);
               }
            }
          }
      }

      if (searchListView && searchListView.count) {
          if (searchListView.count === 0 && searchField.text !== "") {
              noResultsColumn.visible = true
          } else {
              noResultsColumn.visible = false
          }
      } else {
          noResultsColumn.visible = true
      }
    }

    function getLinesForStationResultHandler(result) {
        var jsonResult = JSON.parse(result.toString())
        Functions.log("json result (linesForStation) : " + result)

        if (jsonResult && jsonResult.lines && jsonResult.lines.length > 0) {
            console.log("line results : " + jsonResult.lines.length)
            pageStack.push(Qt.resolvedUrl("StationLinesPage.qml"), {
                               stationLineList: jsonResult,
                               stationId: stationId,
                               stationName: stationName
                           })
        }
    }

    function errorResultHandler(result) {
        stationSearchNotification.show(result)
    }

    AppNotification {
        id: stationSearchNotification
    }

    SilicaFlickable {
        id: stationSearchFlickable

        anchors.fill: parent
        contentHeight: parent.height
        contentWidth: parent.width

        Column {
            id: searchColumn

            Behavior on opacity {
                NumberAnimation {
                }
            }

            width: parent.width

            Timer {
                id: searchTimer
                interval: 800
                running: false
                repeat: false
                onTriggered: {
                    searchResultListModel.clear()
                    getDataBackend(Constants.BACKEND_STUTTGART).searchStation(searchField.text);
                }
            }

            PageHeader {
                id: searchHeader
                //: StationSearchPage search result header
                title: qsTr("Search Results")
            }

            SearchField {
                id: searchField
                width: parent.width
                //: StationSearchPage search result input field
                placeholderText: qsTr("Find your Station...")
                focus: true

                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: focus = false

                onTextChanged: {
                    var searchFieldLength = Functions.calculateVisibleStringLength(
                                searchField.text)
                    if (searchFieldLength > 1) {
                        // only start search if we have at least 2 characters
                        searchTimer.stop()
                        searchTimer.start()
                    } else {
                        noResultsColumn.visible = false
                        searchResultListModel.clear()
                    }
                }
            }

            Column {
                height: stationSearchPage.height - searchHeader.height - searchField.height
                width: parent.width

                id: noResultsColumn
                Behavior on opacity {
                    NumberAnimation {
                    }
                }
                opacity: visible ? 1 : 0
                visible: false

                Label {
                    id: noResultsLabel
                    anchors.horizontalCenter: parent.horizontalCenter
                    //: StationSearchPage no results label
                    text: qsTr("No results found")
                    color: Theme.secondaryColor
                }
            }

            SilicaListView {
                id: searchListView

                height: stationSearchPage.height - searchHeader.height - searchField.height
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right
                opacity: (searchListView.count === 0
                          && Functions.calculateVisibleStringLength(
                              searchField.text) > 1) ? 0 : 1
                visible: (searchListView.count === 0
                          && Functions.calculateVisibleStringLength(
                              searchField.text) > 1) ? false : true

                Behavior on opacity {
                    NumberAnimation {
                    }
                }

                clip: true

                model: ListModel {
                    id: searchResultListModel
                }

                delegate: StationListItem {
                    onClicked: {
                        var selectedItem = searchResultListModel.get(index)
                        console.log("selected index : "+ index + ", item : " + JSON.stringify(selectedItem))
                        stationName = selectedItem.disassembledName;
                        stationId = selectedItem.id;
                        getDataBackend(Constants.BACKEND_STUTTGART).getLinesForStation(selectedItem.id);
                    }
                }

                VerticalScrollDecorator {
                }
            }
        }
    }

    Component.onCompleted: {
         connectSlots();
    }

    Component.onDestruction: {
         disconnectSlots();
    }

}
