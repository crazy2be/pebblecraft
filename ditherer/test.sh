#!/bin/bash -e

./build.sh
./ditherer.bin ../converter/dump.bin
(
../viewer/viewer.bin grey1 dithered.bin &
../viewer/viewer.bin grey4 ../converter/dump.bin
)
