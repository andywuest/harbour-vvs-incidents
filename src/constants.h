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
const char USER_AGENT[] = "Mozilla/5.0 (X11; Linux x86_64; rv:146.0) Gecko/20100101 Firefox/146.0";

const char INCIDENTS_VVS_URL[] = "https://www3.vvs.de/mngvvs/XML_ADDINFO_REQUEST?AIXMLReduction=removeSourceSystem&SpEncId=0&coordOutputFormat=EPSG:4326&filterMessageSubtype=disruption:lines&filterMessageSubtype=disruption:stops&filterPublicationStatus=current&filterShowLineList=0&filterShowPlaceList=0&filterShowStopList=0&outputFormat=rapidJSON&serverInfo=1&version=10.2.10.139";

const char STATIONS_VVS_URL[] = "https://www3.vvs.de/mngvvs/XML_STOPFINDER_REQUEST?SpEncId=0&coordOutputFormat=EPSG:4326&name_sf=%1&outputFormat=rapidJSON&serverInfo=1&suggestApp=vvs&type_sf=any&version=10.2.10.139";

// %1 - e.g. "de:08111:6073" is id response of STATIONS_VVS_URL
const char LINES_FOR_STATION_URL[] = "https://www3.vvs.de/mngvvs/XML_SERVINGLINES_REQUEST?SpEncId=0&command=direct&coordOutputFormat=EPSG:4326&deleteAssignedStops=1&lineReqType=2&locationServerActive=1&lsShowTrainsExplicit=1&mergeDir=0&mode=odv&name_sl=%1&net=vvs&outputFormat=rapidJSON&serverInfo=1&type_sl=stopID&version=10.2.10.139";

// %1 - e.g. "vvs:20007: :R:j26:1"
// %2 - e.g. "de:08116:2972"
const char STATION_LINE_PLAN_JSON_URL[] = "https://www3.vvs.de/mngvvs/XML_STT_REQUEST?SpEncId=0&allStopInfo=1&coordOutputFormat=EPSG:4326&line=%1&mode=direct&name_stt=%2&outputFormat=rapidJSON&serverInfo=1&type_stt=stopID";

// %1 - e.g. "/vvsefaall/AHF/efa12.dc.vvs.de__000135f.pdf"
const char DOWNLOAD_URL[] = "https://www3.vvs.de//%1";

#endif // CONSTANTS_H
