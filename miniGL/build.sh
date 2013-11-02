#!/bin/bash -e

CFLAGS="-std=c99 -DDESKTOP=1 -O0 -g -Wall -Wfatal-errors"
CC=gcc

rm -f main.bin

MINIGL_SRC=(lib/fastgraph.c lib/fastmath.c lib/miniGL.c  lib/miniGL_lib.c lib/mine.c)

MSRC_OBJS=""
for MSRC in ${MINIGL_SRC[@]}; do 
    $CC $CFLAGS $INCLUDE_PATH $DEFINES $MSRC -c -o ${MSRC}.o
    MSRC_OBJS+="${MSRC}.o "
done

$CC $CFLAGS $INCLUDE_PATH $DEFINES main.c ${MSRC_OBJS[@]} -o main.bin

