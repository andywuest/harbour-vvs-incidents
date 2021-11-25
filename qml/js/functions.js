
Qt.include('constants.js');

function getDataBackend(backendId) {
    if (BACKEND_STUTTGART === backendId) {
        console.log("backend stuttgart : " + backendStuttgart)
        return backendStuttgart;
    }
}

function log(message) {
    if (loggingEnabled && message) {
        console.log(message);
    }
}

function resolveIconForLines(affectedLines) {
    for (var i = 0; i < affectedLines.lines.length; i++) {
        if (affectedLines.lines[i].name.indexOf("Bus") !== -1) {
            return "bus"
        }
    }
    return "???";
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
