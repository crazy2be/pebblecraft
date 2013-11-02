#!/bin/bash -e

./build.sh
./main.bin
../viewer/viewer.bin grey4 fb_001.gray4bit 
