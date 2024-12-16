#!/bin/sh

echo "Getting Unicode data ... "
addvar UNICODE 16.0.0

wget -nc "http://www.unicode.org/Public/zipped/$UNICODE/UCD.zip"
unzip -n -j UCD.zip UnicodeData.txt -d share
