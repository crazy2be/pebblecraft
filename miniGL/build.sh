
CFLAGS="-std=c99 -DDEBUG=1 -O0 -g -Wall -Wfatal-errors"
CC=gcc

rm -f main.bin

MINIGL_SRC=( fastgraph.c  fastmath.c  miniGL.c  miniGL_lib.c )

MSRC_OBJS=""
for MSRC in ${MINIGL_SRC[@]}; do 
    $CC $CFLAGS $INCLUDE_PATH $DEFINES $MSRC -c -o ${MSRC}.o
    MSRC_OBJS+="${MSRC}.o "
done

$CC $CFLAGS $INCLUDE_PATH $DEFINES main.c ${MSRC_OBJS[@]} -o main.bin

