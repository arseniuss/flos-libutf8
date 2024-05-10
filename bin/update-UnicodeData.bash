#!/bin/bash

UNICODE_VER=$1

wget -nc -q "http://www.unicode.org/Public/zipped/$UNICODE_VER/UCD.zip"
unzip -q -j UCD.zip UnicodeData.txt -d share
