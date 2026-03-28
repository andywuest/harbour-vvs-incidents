import QtQuick 2.2
import QtQuick.LocalStorage 2.0
import Sailfish.Silica 1.0

import "../js/constants.js" as Constants
import "../js/functions.js" as Functions

import "../components"
import "../components/thirdparty"

Page {
    id: stationLinesPage

    property var stationLineList;

    allowedOrientations: Orientation.All

    function connectSlots() {
        Functions.log("StationLinesPage - connecting - slots")
    }

    function disconnectSlots() {
        Functions.log("StationLinesPage - disconnecting - slots")
    }

    function populateModel() {
        stationLinesListModel.clear()
        if (stationLineList && stationLineList.result) {
            Functions.log("StationLinesPage - populating model")
            for (var i = 0; i < stationLineList.result.length; i++) {
                Functions.log("StationLinesPage - adding : " + stationLineList.result[i])
                stationLinesListModel.append(stationLineList.result[i]);
            }
        }
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

            width: parent.width

            PageHeader {
                id: searchHeader
                //: StationLinesPage search result header
                title: qsTr("Lines");
                description: "TODO";
            }

            SilicaListView {
                id: searchListView

                height: stationSearchFlickable.height - searchHeader.height
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right

                clip: true

                model: ListModel {
                    id: stationLinesListModel
                }

                delegate: StationLineItem {
                    onClicked: {
                        var selectedItem = stationLinesListModel.get(index)
                        console.log("selected index : "+ index + ", item : " + selectedItem + ", value " + value)
                    }
                }

                VerticalScrollDecorator {
                }
            }
        }
    }

    Component.onCompleted: {
        console.log("[StationLinesPage] init");
        populateModel();
         connectSlots();
    }

    Component.onDestruction: {
        console.log("[StationLinesPage] destroy");
        stationLineList = {};
         disconnectSlots();
    }

}
