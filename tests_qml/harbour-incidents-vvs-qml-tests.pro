CONFIG += warn_on qmltestcase

TEMPLATE = app

DISTFILES += \
    testdata/rbahn.json \
    testdata/zacke.json \
    tst_functions.qml

SOURCES += \
    main.cpp

OTHER_FILES += \
    testdata/sbahn.json

