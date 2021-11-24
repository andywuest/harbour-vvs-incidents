/**
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
#ifndef CONSTANTS_H
#define CONSTANTS_H

const char MIME_TYPE_JSON[] = "application/json";
const char USER_AGENT[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:73.0) Gecko/20100101 Firefox/73.0";

const char INCIDENTS_VVS_URL[] = "https://www3.vvs.de/mngvvs/XML_ADDINFO_REQUEST?AIXMLReduction=removeSourceSystem&SpEncId=0&coordOutputFormat=EPSG:4326&filterMessageSubtype=disruption:lines&filterMessageSubtype=disruption:stops&filterPublicationStatus=current&filterShowLineList=0&filterShowPlaceList=0&filterShowStopList=0&outputFormat=rapidJSON&serverInfo=1&version=10.2.10.139";

#endif // CONSTANTS_H
