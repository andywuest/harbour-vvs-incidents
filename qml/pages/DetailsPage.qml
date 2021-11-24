/*
 * harbour-watchlist - Sailfish OS Version
 * Copyright © 2020 Andreas Wüst (andreas.wuest.freelancer@gmail.com)
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

import "../js/functions.js" as Functions

Page {
    id: detailsPage
    property var incident

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors {
            fill: parent
            bottomMargin: Theme.paddingMedium
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x
            spacing: Theme.paddingSmall

            PageHeader {
                //: DetailsPage incident page header
                title: qsTr("Details")
            }

            Row {
                id: lineRow
                width: parent.width
                height: Theme.fontSizeSmall + Theme.paddingMedium

                Image {
                   id: rowIcon
                   source: "../icons/" + "vvs_" + Functions.resolveIconForLines(incident.affected) + ".svg"
                   height: lineRow.height
                   width: lineRow.height
                   anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    id: lineLabel
                    // width: parent.width * 8 / 10
                    height: parent.height
                    width: lineRow.width - rowIcon.width
                    text: Functions.getListOfAffectedLines(incident.affected)
                    truncationMode: TruncationMode.Fade// TODO check for very long texts
                    // elide: Text.ElideRight
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeSmall
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                }

            }

//            Label {
//                id: lineLabel
//                text: Functions.getListOfAffectedLines(incident.affected)
//                width: parent.width
//                wrapMode: Text.WordWrap
//                font.pixelSize: Theme.fontSizeMedium
//            }

            Label {
                id: headlineLabel
                text: incident.urlText
                width: parent.width
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                id: dateTimeSourceLabel
                text: incident._timestampFormatted
                // "Timestamp"
                // newsItem.dateTime + " | " + newsItem.source
                textFormat: Text.RichText
                width: parent.width
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeTiny
            }

            Label {
                id: contentLabel
                text: incident.properties.htmlText
                textFormat: Text.RichText
                width: parent.width
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }
    }

}
