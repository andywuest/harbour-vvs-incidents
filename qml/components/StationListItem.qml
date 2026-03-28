import QtQuick 2.2
import Sailfish.Silica 1.0

import "../../qml/js/functions.js" as Functions

ListItem {
    id: stationListItem

    contentHeight: stationItem.height + (2 * Theme.paddingSmall)
    contentWidth: parent.width

    Item {
        id: stationItem
        height: resultColumn.height
        width: parent.width - (2 * Theme.paddingMedium)
        x: Theme.paddingMedium
        y: Theme.paddingSmall

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
                affectedLines: "" + (object ? object : name)
            }

            Row {
                id: genericAdditionalInfoRow
                height: Theme.fontSizeMedium
                width: parent.width

                Text {
                    id: locationNameText
                    width: parent.width
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    text: ref.place
                    textFormat: Text.StyledText
                    elide: Text.ElideRight
                    maximumLineCount: 1
                }
            }
        }
    }

}
