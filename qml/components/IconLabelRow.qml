import QtQuick 2.6
import Sailfish.Silica 1.0

Row {
    id: iconLabelRow
    width: parent.width
    height: Theme.fontSizeSmall + Theme.paddingMedium

    property string lineType: ""
    property string affectedLines: ""

    Image {
       id: rowIcon
       source: "../icons/" + "vvs_" + lineType + ".svg"
       height: iconLabelRow.height
       width: iconLabelRow.height
       fillMode: Image.PreserveAspectFit
       anchors.verticalCenter: parent.verticalCenter
    }

    Label {
        id: marginLabel
        width: Theme.paddingSmall
    }

    Label {
        id: rowLabel
        height: parent.height
        width: iconLabelRow.width - rowIcon.width - marginLabel.width
        text: affectedLines
        truncationMode: TruncationMode.Fade
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeSmall
        font.bold: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}
