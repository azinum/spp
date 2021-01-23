# config.mk
# spp - simple preprocessor

CC=gcc

PROG=spp

BUILD_DIR=build

SRC=src/*.c

INC_DIR=include

INC=${INC_DIR}/*.h

FLAGS=${SRC} -o ${BUILD_DIR}/${PROG} -I${INC_DIR} -Wall -O2
