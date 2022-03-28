.pragma library

Qt.include('constants.js');

function log(message) {
    if (loggingEnabled && message) {
        console.log(message);
    }
}

function resolveIconForLines(affectedLines) {
    for (var i = 0; i < affectedLines.lines.length; i++) {
        var lineName = affectedLines.lines[i].name;
        if (containsSubstring(lineName, "Bus")) {
            return "bus"
        } else if (containsSubstring(lineName, "Nachtbus")) {
            return "bus"
        } else if (containsSubstring(lineName, "Zahnradbahn")) {
            return "zacke"
        } else if (containsSubstring(lineName, "S-Bahn")) {
            return "sbahn"
        } else if (containsSubstring(lineName, "R-Bahn")) {
            return "rbahn"
        } else if (containsSubstring(lineName, "Stadtbahn")) {
            return "ubahn"
        }
    }
    return "???";
}

function containsSubstring(string, subString) {
    return string.indexOf(subString) !== -1;
}

function getListOfAffectedLines(affectedLines) {
    var results = [];
    for (var i = 0; i < affectedLines.lines.length; i++) {
        var lineNumber = affectedLines.lines[i].number;
        if (results.indexOf(lineNumber) === -1) {
            results.push(lineNumber)
        }
    }
    log("list of affected lines : " + results);
    return results.join(', ');
}

function createAvailabilityLabel(from, to) {
    var label = "";
    if (from === to) {
        label = qsTr("On %1").arg(from);
    } else if (to === "")   {
        label = qsTr("From %1 until further notice").arg(from);
    }  else if (from !== to) {
        label = qsTr("From %1 until %2").arg(from).arg(to);
    }
    return label;
}

