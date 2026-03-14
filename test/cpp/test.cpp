#include "test_HtmlDataExtractor.h"

void Test_HtmlDataExtractor::initTestCase()
{
    extractor = new HtmlDataExtractor();
    QVERIFY2(extractor, "Could not create HtmlDataExtractor");
}

void Test_HtmlDataExtractor::cleanupTestCase()
{
    delete extractor;
}

void Test_HtmlDataExtractor::testEmptyHtml()
{
    QJsonObject result = extractor->parseLinienSelectToJson("");
    QCOMPARE(result["result"].toArray().size(), 0);
}

void Test_HtmlDataExtractor::testNoLinienSelect()
{
    QString html = "<div>no select here</div>";
    QJsonObject result = extractor->parseLinienSelectToJson(html);
    QCOMPARE(result["result"].toArray().size(), 0);
}

void Test_HtmlDataExtractor::testSimpleSelectWithOptions()
{
    QString html = R"(
        <select id="linien">
            <option value="10">Linie 10</option>
            <option value="20">Linie 20 nach Stadtmitte</option>
        </select>
    )";

    QJsonObject result = extractor->parseLinienSelectToJson(html);
    QJsonArray array = result["result"].toArray();

    QCOMPARE(array.size(), 2);

    // First option
    QJsonObject opt1 = array[0].toObject();
    QCOMPARE(opt1["value"].toString(), QString("10"));
    QCOMPARE(opt1["name"].toString(), QString("Linie 10"));

    // Second option
    QJsonObject opt2 = array[1].toObject();
    QCOMPARE(opt2["value"].toString(), QString("20"));
    QCOMPARE(opt2["name"].toString(), QString("Linie 20 nach Stadtmitte"));
}

void Test_HtmlDataExtractor::testMultipleSelects()
{
    QString html = R"(
        <select id="other">ignore</select>
        <select id="linien">
            <option value="5">Linie 5</option>
        </select>
    )";

    QJsonObject result = extractor->parseLinienSelectToJson(html);
    QJsonArray array = result["result"].toArray();

    QCOMPARE(array.size(), 1);
    QJsonObject opt = array[0].toObject();
    QCOMPARE(opt["value"].toString(), QString("5"));
    QCOMPARE(opt["name"].toString(), QString("Linie 5"));
}

void Test_HtmlDataExtractor::testOptionsWithoutValue()
{
    QString html = R"(
        <select id="linien">
            <option value="30">Linie 30</option>
            <option>Linie ohne value</option>
        </select>
    )";

    QJsonObject result = extractor->parseLinienSelectToJson(html);
    QJsonArray array = result["result"].toArray();

    // Should only match options WITH value="" attribute
    QCOMPARE(array.size(), 1);
    QJsonObject opt = array[0].toObject();
    QCOMPARE(opt["value"].toString(), QString("30"));
}

void Test_HtmlDataExtractor::testNestedHtmlTagsInOption()
{
    QString html = R"(
        <select id="linien">
            <option value="40">
                <b>Linie 40</b> <span>nach &Uuml;berall</span>
            </option>
        </select>
    )";

    QJsonObject result = extractor->parseLinienSelectToJson(html);
    QJsonObject opt = result["result"].toArray()[0].toObject();

    QCOMPARE(opt["value"].toString(), QString("40"));
    // Nested tags are stripped from name
    QCOMPARE(opt["name"].toString(), QString("Linie 40 nach Überall"));
}
