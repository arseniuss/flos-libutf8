#!/bin/bash

UNICODE_VER=14.0.0

wget -nc -q "http://www.unicode.org/Public/zipped/$UNICODE_VER/UCD.zip"
unzip -q -j UCD.zip UnicodeData.txt -d share
