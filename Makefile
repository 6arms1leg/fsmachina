BUILD_CONTEXT := .
BUILD_DIR := $(BUILD_CONTEXT)/build
SRC_DIR := $(BUILD_CONTEXT)/src

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
        clean-test \
        clean-doc-doxygen \
        clean-doc-uml \
        clean-misra \
        clean-metrics-cmplx \
        clean-metrics-loc \
        clean-all

# Default target
help:
	@echo "Make:  Available targets are:"
	@echo "Make:  * help"
	@echo "Make:  * test"
	@echo "Make:  * test-cov"
	@echo "Make:  * doc-doxygen"
	@echo "Make:  * doc-uml"
	@echo "Make:  * check-misra"
	@echo "Make:  * check-metrics-cmplx"
	@echo "Make:  * check-metrics-loc"
	@echo "Make:  * check-static"
	@echo "Make:  * clean-test"
	@echo "Make:  * clean-doc"
	@echo "Make:  * clean-misra"
	@echo "Make:  * clean-metrics-cmplx"
	@echo "Make:  * clean-metrics-loc"
	@echo "Make:  * clean-all"

test:
	ceedling test:all

test-cov:
	ceedling gcov:all

doc-doxygen:
	mkdir -p $(BUILD_DIR)/doxygen/
	doxygen $(BUILD_CONTEXT)/doc/doxygen/doxygen.conf

doc-uml:
	mkdir -p $(BUILD_DIR)/umlet/
	umlet -action=convert \
	      -format=png \
	      -filename=$(BUILD_CONTEXT)/doc/arc/figures/*.uxf
	mv $(BUILD_CONTEXT)/doc/arc/figures/*.png $(BUILD_DIR)/umlet/

check-misra:
	mkdir -p $(BUILD_DIR)/cppcheck/misra/out/ \
	         $(BUILD_DIR)/cppcheck/misra/result/
	cppcheck --dump $(SRC_DIR)/
	find $(SRC_DIR)/ -type f -name '*.dump' \
	    -exec mv {} $(BUILD_DIR)/cppcheck/misra/out/ \;
	python $(CPPCHECKMISRA) \
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

clean-test:
	ceedling clobber

clean-doc-doxygen:
	rm -rf $(BUILD_DIR)/doxygen/html/*

clean-doc-uml:
	rm -rf $(BUILD_DIR)/umlet/*

clean-misra:
	rm -rf $(BUILD_DIR)/cppcheck/misra/out/* \
	       $(BUILD_DIR)/cppcheck/misra/result/*

clean-metrics-cmplx:
	rm -rf $(BUILD_DIR)/lizard/result/*

clean-metrics-loc:
	rm -rf $(BUILD_DIR)/cloc/result/*

clean-all:
	@echo "Make:  Cleanup all ..."
	$(MAKE) clean-test
	$(MAKE) clean-doc-doxygen
	$(MAKE) clean-doc-uml
	$(MAKE) clean-misra
	$(MAKE) clean-metrics-cmplx
	$(MAKE) clean-metrics-loc
