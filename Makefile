# Makefile - Wrapper for CMake build system

.PHONY: all clean test bench configure
.PHONY: test64_0 test64_1 test128_0 test128_1 test192_0 test192_1 test256_0 test256_1 test512_0 test512_1
.PHONY: bench64_0 bench64_1 bench128_0 bench128_1 bench192_0 bench192_1 bench256_0 bench256_1 bench512_0 bench512_1

all: configure
	@cmake --build build -j4

configure:
	@mkdir -p build
	@cd build && cmake ..

test: all
	@for i in 0 1; do \
		for size in 64 128 192 256 512; do \
			./build/test$${size}_$$i; \
		done; \
	done

bench: all
	@for i in 0 1; do \
		for size in 64 128 192 256 512; do \
			./build/bench$${size}_$$i; \
		done; \
	done

# Individual test targets
test64_0 test64_1 test128_0 test128_1 test192_0 test192_1 test256_0 test256_1 test512_0 test512_1: configure
	@cmake --build build --target $@ -j4
	@./build/$@

# Individual benchmark targets
bench64_0 bench64_1 bench128_0 bench128_1 bench192_0 bench192_1 bench256_0 bench256_1 bench512_0 bench512_1: configure
	@cmake --build build --target $@ -j4
	@./build/$@

clean:
	@rm -rf build





