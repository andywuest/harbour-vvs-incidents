#/bin/bash

find . -name "*.xml" -exec rm  {} \;

env LC_ALL=de_DE.UTF-8 LC_NUMERIC=de_DE.utf8 qmltestrunner -o qmlresults.xml,xml

