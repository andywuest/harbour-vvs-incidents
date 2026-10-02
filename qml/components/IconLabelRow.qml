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
