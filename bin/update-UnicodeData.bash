#!/bin/bash

UNICODE_VER=$1

wget -nc "http://www.unicode.org/Public/zipped/$UNICODE_VER/UCD.zip"
unzip -j UCD.zip UnicodeData.txt -d share
