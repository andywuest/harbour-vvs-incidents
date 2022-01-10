import QtQuick 2.6
import Sailfish.Silica 1.0

Column {
    id: loadingColumn

    width: parent.width - 2 * Theme.horizontalPageMargin
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    spacing: Theme.paddingMedium
    Behavior on opacity {
        NumberAnimation {
        }
    }
    opacity: loadingColumn.visible ? 1 : 0
    InfoLabel {
        id: loadingLabel
        text: qsTr("Loading...")
        font.pixelSize: Theme.fontSizeMedium
    }
}
