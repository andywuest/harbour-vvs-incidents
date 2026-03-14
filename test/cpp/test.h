#ifndef TEST_HTMLDATAEXTRACTOR_H
#define TEST_HTMLDATAEXTRACTOR_H

#include <QObject>
#include <QTest>
#include <QJsonObject>
#include "HtmlDataExtractor.h"

class Test_HtmlDataExtractor : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testEmptyHtml();
    void testNoLinienSelect();
    void testSimpleSelectWithOptions();
    void testMultipleSelects();
    void testOptionsWithoutValue();
    void testNestedHtmlTagsInOption();

private:
    HtmlDataExtractor *extractor;
};

#endif // TEST_HTMLDATAEXTRACTOR_H
