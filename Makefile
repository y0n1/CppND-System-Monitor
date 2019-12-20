.PHONY: all
all: format test build run

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: test
test:
	echo "No test to run yet..."

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: clean
clean:
	rm -rf build

.PHONY: run
run: 
	[ -f build/monitor ] && build/monitor
