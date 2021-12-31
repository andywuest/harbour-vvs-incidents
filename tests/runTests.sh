#/bin/bash

rm *.o
rm Makefile
rm moc_*

qmake 
make

ls -l
pwd

./BackendStuttgartTests -o cppresults.xml,xml

