BUILD_CONTEXT := .
BUILD_DIR := $(BUILD_CONTEXT)/build
SRC_DIR := $(BUILD_CONTEXT)/src

CC := gcc
CC_FLAGS := -Wall \
            -Wextra \
            -Wpedantic \
            -Wredundant-decls \
            -Wmissing-declarations \
            -Wmissing-prototypes \
            -Wstrict-prototypes \
            -Wdouble-promotion \
            -Wcast-align \
            -Wcast-qual \
            -Wconversion \
            -Wwrite-strings \
            -Wparentheses \
            -Wshadow \
            -Werror \
            -std=c99 \
            -Og \
            -g
LD := gcc
LD_FLAGS := 

ifndef CPPCHECKMISRA
CPPCHECKMISRA := /usr/share/cppcheck/addons/misra.py
endif

.PHONY: help \
        test \
        test-cov \
        doc-doxygen \
        doc-uml \
        check-misra \
        check-metrics-cmplx \
        check-metrics-loc \
        check-static \
        clean-ex-app \
        clean-test \
        clean-doc-doxygen \
        clean-doc-uml \
        clean-misra \
        clean-metrics-cmplx \
        clean-metrics-loc

# Default target
help:
	@echo "Make:  Available targets are:"
	@echo "Make:  * help"
	@echo "Make:  * ex-app"
	@echo "Make:  * test"
	@echo "Make:  * test-cov"
	@echo "Make:  * doc-doxygen"
	@echo "Make:  * doc-uml"
	@echo "Make:  * check-misra"
	@echo "Make:  * check-metrics-cmplx"
	@echo "Make:  * check-metrics-loc"
	@echo "Make:  * check-static"
	@echo "Make:  * clean-ex-app"
	@echo "Make:  * clean-test"
	@echo "Make:  * clean-doc-doxygen"
	@echo "Make:  * clean-doc-uml"
	@echo "Make:  * clean-misra"
	@echo "Make:  * clean-metrics-cmplx"
	@echo "Make:  * clean-metrics-loc"
	@echo "Make:  * clean-all"

$(BUILD_DIR)/fsmachina/obj/SMfsm.o: $(SRC_DIR)/SMfsm.c
	mkdir -p ./build/fsmachina/obj/
	$(CC) \
	    $(CC_FLAGS) \
	    -c \
	    $< \
	    -o $@

$(BUILD_DIR)/fsmachina/obj/SMstatHndlr.o: $(BUILD_CONTEXT)/test/support/SMstatHndlr.c
	mkdir -p ./build/fsmachina/obj/
# Include `ex-app` path first to use its local `assert` header
	$(CC) \
	    $(CC_FLAGS) \
	    -c \
	    -I$(BUILD_CONTEXT)/ex-app/ \
	    -I$(SRC_DIR)/ \
	    $< \
	    -o $@

$(BUILD_DIR)/fsmachina/obj/SMactivity.o: $(BUILD_CONTEXT)/ex-app/SMactivity.c
	mkdir -p ./build/fsmachina/obj/
	$(CC) \
	    $(CC_FLAGS) \
	    -c \
	    $< \
	    -o $@

$(BUILD_DIR)/fsmachina/obj/main.o: $(BUILD_CONTEXT)/ex-app/main.c
	mkdir -p ./build/fsmachina/obj/
# Include `ex-app` path first to use its local `assert` header
	$(CC) \
	    $(CC_FLAGS) \
	    -c \
	    -I$(BUILD_CONTEXT)/ex-app/ \
	    -I$(SRC_DIR)/ \
	    -I$(BUILD_CONTEXT)/test/support/ \
	    $< \
	    -o $@

ex-app: $(BUILD_DIR)/fsmachina/ex-app
$(BUILD_DIR)/fsmachina/ex-app: $(BUILD_DIR)/fsmachina/obj/SMfsm.o \
                               $(BUILD_DIR)/fsmachina/obj/SMstatHndlr.o \
                               $(BUILD_DIR)/fsmachina/obj/SMactivity.o \
                               $(BUILD_DIR)/fsmachina/obj/main.o
	$(LD) $(LD_FLAGS) $^ -o $@

test:
	ceedling test:all

test-cov:
	ceedling gcov:all

doc-doxygen:
	mkdir -p $(BUILD_DIR)/doxygen/
	doxygen $(BUILD_CONTEXT)/doc/doxygen/doxygen.conf

doc-uml:
	mkdir -p $(BUILD_DIR)/umlet/
	umlet \
	    -action=convert \
	    -format=png \
	    -filename=$(BUILD_CONTEXT)/doc/arc/figures/*.uxf
	mv $(BUILD_CONTEXT)/doc/arc/figures/*.png $(BUILD_DIR)/umlet/

check-misra:
	mkdir -p \
	    $(BUILD_DIR)/cppcheck/misra/out/ \
	    $(BUILD_DIR)/cppcheck/misra/result/
	cppcheck --dump $(SRC_DIR)/
	find \
	    $(SRC_DIR)/ -type f -name '*.dump' \
	    -exec mv {} $(BUILD_DIR)/cppcheck/misra/out/ \;
	python3 $(CPPCHECKMISRA) \
	    $(BUILD_DIR)/cppcheck/misra/out/*.dump \
	    2>&1 | tee $(BUILD_DIR)/cppcheck/misra/result/misra-result.txt
	@grep \
	    -q 'MISRA rules violations found:' \
	    $(BUILD_DIR)/cppcheck/misra/result/misra-result.txt && \
	    exit 1 || exit 0

check-metrics-cmplx:
	mkdir -p $(BUILD_DIR)/lizard/result/
	lizard \
	    -l c \
	    -C 10 \
	    -a 6 \
	    -ENS -Tmax_nested_structures=5 \
	    $(SRC_DIR)/ \
	    2>&1 | tee $(BUILD_DIR)/lizard/result/lizard-result.txt
	@grep \
	    -q '!!!! Warnings' \
	    $(BUILD_DIR)/lizard/result/lizard-result.txt && \
	    exit 1 || exit 0

check-metrics-loc:
	mkdir -p $(BUILD_DIR)/cloc/result/
	cloc \
	    --by-file \
	    --by-percent cm \
	    $(SRC_DIR)/ \
	    2>&1 | tee $(BUILD_DIR)/cloc/result/cloc-result.txt

check-static:
	cppcheck --verbose --error-exitcode=1 $(SRC_DIR)/

clean-ex-app:
	rm -rf \
	    $(BUILD_DIR)/fsmachina/obj/*.o \
	    $(BUILD_DIR)/fsmachina/ex-app

clean-test:
	ceedling clobber

clean-doc-doxygen:
	rm -rf $(BUILD_DIR)/doxygen/html/*

clean-doc-uml:
	rm -rf $(BUILD_DIR)/umlet/*

clean-misra:
	rm -rf \
	    $(BUILD_DIR)/cppcheck/misra/out/* \
	    $(BUILD_DIR)/cppcheck/misra/result/*

clean-metrics-cmplx:
	rm -rf $(BUILD_DIR)/lizard/result/*

clean-metrics-loc:
	rm -rf $(BUILD_DIR)/cloc/result/*

clean-all: clean-test \
           clean-doc-doxygen \
           clean-doc-uml \
           clean-misra \
           clean-metrics-cmplx \
           clean-metrics-loc
	@echo "Make:  All cleaned up"
