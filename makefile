# makefile
# spp

include config.mk

all: prepare compile run

prepare: ${BUILD_DIR}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

compile: ${SRC} ${INC}
	${CC} ${FLAGS}

run:
	./${BUILD_DIR}/${PROG}

install:
