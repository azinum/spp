# makefile
# spp

include config.mk

all: prepare compile run

prepare:
	@mkdir -p ${BUILD_DIR}
	@mkdir -p ${OUT_DIR}

compile: ${SRC}
	${CC} ${FLAGS}

self: prepare
	@mkdir -p ${OUT_DIR}/src
	@mkdir -p ${OUT_DIR}/include
	@cp makefile config.mk ${OUT_DIR}
	@for file in ${SRC} ${INC} ; do \
		spp $${file} > ${OUT_DIR}/$${file} ; \
		echo "Parsed $${file} -> ${OUT_DIR}/$${file}"; \
	done

run:
	./${BUILD_DIR}/${PROG} test.c # > ${OUT_DIR}/out.c

install:
	${CC} ${FLAGS}
	chmod o+x ${BUILD_DIR}/${PROG}
	cp ${BUILD_DIR}/${PROG} ${INSTALL_DIR}/${PROG}

uninstall:
	rm ${INSTALL_DIR}/${PROG}

clean:
	rm -dr ${OUT_DIR}
