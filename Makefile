default: build test

.PHONY: prep_merge # Prepare for merging
prep_merge: cppcheck clean build test man_pages memcheck

.PHONY: cppcheck # Run cppcheck
cppcheck:
	@echo "Running cppcheck..."
	@cppcheck --std=c++17 --language=c++ --enable=all \
		-q --error-exitcode=255 \
		-I . -I spoc -I apps -I laslib/LASlib/inc \
		--inline-suppr \
		--suppress=missingIncludeSystem \
		--suppress='*:laslib/LASlib/inc/*' \
		apps/*/*.cpp

# Run cmake when CMakeLists.txt changes
./build/debug/Makefile: CMakeLists.txt
	@echo "Running cmake..."
	@mkdir -p build/debug
	@mkdir -p build/release
	@cd build/debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..
	@cd build/release && cmake -DCMAKE_BUILD_TYPE=Release ../..

.PHONY: laslib # Build LASlib library
laslib:
	$(MAKE) -j -C laslib/LASlib

.PHONY: build # Compile all applications and tests
build: ./build/debug/Makefile laslib
	cd build/debug && make -j 8
	cd build/release && make -j 8

.PHONY: clean # Clean build objects
clean:
	@echo "Cleaning..."
	@rm -rf build

.PHONY: app_test
app_test: BUILD=debug
app_test:
	@parallel --jobs 24 --halt now,fail=1 "echo {} && {}" ::: build/$(BUILD)/test_*

.PHONY: integration_test
integration_test: BUILD=debug
integration_test:
	@parallel --jobs 24 --halt now,fail=1 \
		"echo $(BUILD): {} && PATH=./build/$(BUILD)/:$$PATH {}" ::: tests/integration/test_*.sh

.PHONY: test # Run tests
test:
	@echo "Testing..."
	@$(MAKE) --no-print-directory app_test BUILD=debug
	@$(MAKE) --no-print-directory app_test BUILD=release
	@$(MAKE) --no-print-directory integration_test BUILD=debug
	@$(MAKE) --no-print-directory integration_test BUILD=release

.PHONY: memcheck # Run memcheck
memcheck:
	@echo "Running memchecks..."
	@parallel --jobs 24 --halt now,fail=1 \
		"echo {} && valgrind --leak-check=full --error-exitcode=1 --quiet --suppressions=valgrind.suppressions {}" ::: build/debug/test_*
	@parallel --jobs 24 --halt now,fail=1 \
		"echo {} && valgrind --leak-check=full --error-exitcode=1 --quiet --suppressions=valgrind.suppressions {}" ::: build/release/test_*

.PHONY: man_pages # Generate man pages
man_pages:
	@mkdir -p build/man
	@find apps/*/*.md | xargs --verbose -P 8 -I {} bash -c 'pandoc -s -t man {} -o build/man/spoc_`basename {} .md`'

.PHONY: help # Generate list of targets with descriptions
help:
	@grep '^.PHONY: .* #' Makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/\1	\2/' | expand -t20
