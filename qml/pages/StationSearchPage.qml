import QtQuick 2.2
import QtQuick.LocalStorage 2.0
import Sailfish.Silica 1.0

// QTBUG-34418
import "."

import "../js/constants.js" as Constants
import "../js/functions.js" as Functions

import "../components"
import "../components/thirdparty"

Page {
    id: stationSearchPage

    allowedOrientations: Orientation.All

    function connectSlots() {
        Functions.log("StationSearchPage - connecting - slots")
        var dataBackend = getDataBackend(Constants.BACKEND_STUTTGART);
        dataBackend.searchStationResultAvailable.connect(searchStationResultHandler);
        dataBackend.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        Functions.log("StationSearchPage - disconnecting - slots")
        var dataBackend = getDataBackend(Constants.BACKEND_STUTTGART);
        dataBackend.searchStationResultAvailable.disconnect(searchStationResultHandler);
        dataBackend.requestError.disconnect(errorResultHandler);
    }

    function searchStationResultHandler(result) {
      var jsonResult = JSON.parse(result.toString())
      Functions.log("json result from backend was: " + result)

      for (var i = 0; i < jsonResult.points.length; i++)   {
          if (jsonResult.points[i]) {
            searchResultListModel.append(jsonResult.points[i]);
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

                delegate: ListItem {
                    id: delegate


                    Column {
                        id: resultColumn
                        width: parent.width - (2 * Theme.horizontalPageMargin)
                        height: iconLabelRow.height
                                + genericAdditionalInfoRow.height
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        IconLabelRow {
                            id: iconLabelRow
                            lineType: Functions.resolveIconForLocation(anyType)
                            affectedLines: "" + object
                        }

//                        Label {
//                            id: stationNameText
//                            font.pixelSize: Theme.fontSizeSmall
//                            color: Theme.primaryColor
//                            text: object
//                            textFormat: Text.StyledText
//                            //elide: Text.ElideRight
//                            truncationMode: TruncationMode.Fade // TODO check for very long texts
//                            maximumLineCount: 1
//                            width: parent.width
//                            height: Theme.fontSizeMedium
//                            visible: false
//                        }

                        Row {
                            id: genericAdditionalInfoRow
                            height: Theme.fontSizeMedium
                            width: parent.width

                            Text {
                                id: stockSource
                                width: parent.width * 3 / 3
                                font.pixelSize: Theme.fontSizeExtraSmall
                                color: Theme.secondaryColor
                                text: ref.place
                                textFormat: Text.StyledText
                                elide: Text.ElideRight
                                maximumLineCount: 1
                            }
//                            Text {
//                                id: stockIsin
//                                width: parent.width / 3
//                                visible: false
//                                font.pixelSize: Theme.fontSizeExtraSmall
//                                color: Theme.secondaryColor
//                                text: mainLoc // isin - should be available for all backends
//                                textFormat: Text.StyledText
//                                elide: Text.ElideRight
//                                horizontalAlignment: Text.AlignRight
//                                maximumLineCount: 1
//                            }
                        }

                    }


                    onClicked: {
                        var selectedItem = searchResultListModel.get(index)
                        console.log("selected index : "+ index + ", item : " + selectedItem)
                        //var result = Database.persistStockData(selectedItem, watchlistId)
                        //app.securityAdded(watchlistId);
                        // stockAddedNotification.show(result);
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
