import QtQuick 2.0
import QtTest 1.2

import "../qml/js/functions.js" as Functions
import "../qml/js/constants.js" as Constants

TestCase {
    name: "Function Tests"

    property var sbahnData: ({})
    property var rbahnData: ({})
    property var zackeData: ({})

    function loadData(source, callback) {
        var xhr = new XMLHttpRequest;
        xhr.open("GET", source, false); // false - load synchronously
        xhr.onreadystatechange = function () {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                //console.log("data : " + xhr.responseText);
                callback(JSON.parse(xhr.responseText));
            }
        }
        xhr.send()
    }

    function init_data() {
        console.log("reading data")
        loadData("./testdata/sbahn.json", function(jsonData) { sbahnData = jsonData; });
        loadData("./testdata/rbahn.json", function(jsonData) { rbahnData = jsonData; });
        loadData("./testdata/zacke.json", function(jsonData) { zackeData = jsonData; });
    }

    function test_functions_getListOfAffectedLines() {
        compare("S2", Functions.getListOfAffectedLines(sbahnData.affected))
        compare("RB64", Functions.getListOfAffectedLines(rbahnData.affected))
        compare("10", Functions.getListOfAffectedLines(zackeData.affected))
    }

    function test_functions_resolveIconForLines() {
        compare("sbahn", Functions.resolveIconForLines(sbahnData.affected))
        compare("rbahn", Functions.resolveIconForLines(rbahnData.affected))
        compare("zacke", Functions.resolveIconForLines(zackeData.affected))
    }
}
