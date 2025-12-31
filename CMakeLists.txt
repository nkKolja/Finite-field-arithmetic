cmake_minimum_required(VERSION 3.15)
project(FiniteFieldArithmetic C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -ffast-math -Wall -Wextra -Wno-unused-function -Werror")

# Common source files
set(COMMON_SOURCES
    src/random/random.c
    src/arith.c
)

# Test sources
set(TEST_SOURCES
    ${COMMON_SOURCES}
    tests/tests.c
)

# Benchmark sources
set(BENCH_SOURCES
    ${COMMON_SOURCES}
    benchmarks/bench.c
)

# Option for optimization level
option(USE_ARM_OPTIMIZATIONS "Use ARM64 assembly optimizations" OFF)

# Define all 10 prime variants
set(PRIME_CONFIGS
    "64_0:P64_0:p64_0:64"
    "64_1:P64_1:p64_1:64"
    "128_0:P128_0:p128_0:128"
    "128_1:P128_1:p128_1:128"
    "192_0:P192_0:p192_0:192"
    "192_1:P192_1:p192_1:192"
    "256_0:P256_0:p256_0:256"
    "256_1:P256_1:p256_1:256"
    "512_0:P512_0:p512_0:512"
    "512_1:P512_1:p512_1:512"
)

# Create test and bench targets for each prime configuration
foreach(CONFIG ${PRIME_CONFIGS})
    string(REPLACE ":" ";" CONFIG_LIST ${CONFIG})
    list(GET CONFIG_LIST 0 SIZE_VARIANT)
    list(GET CONFIG_LIST 1 PRIME_DEF)
    list(GET CONFIG_LIST 2 DIR_NAME)
    list(GET CONFIG_LIST 3 SIZE_BITS)
    
    set(TEST_TARGET "test${SIZE_VARIANT}")
    set(BENCH_TARGET "bench${SIZE_VARIANT}")
    
    # Determine source files based on optimization level
    if(USE_ARM_OPTIMIZATIONS AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
        set(PRIME_SOURCES
            src/primes/${DIR_NAME}/arm64/arith_arm${SIZE_BITS}.c
            src/primes/${DIR_NAME}/arm64/arith_arm${SIZE_BITS}.S
            src/primes/${DIR_NAME}/prime_params.c
        )
    else()
        set(PRIME_SOURCES
            src/primes/${DIR_NAME}/generic/arith_generic.c
            src/primes/${DIR_NAME}/prime_params.c
        )
    endif()
    
    # Create test executable
    add_executable(${TEST_TARGET}
        ${TEST_SOURCES}
        ${PRIME_SOURCES}
    )
    
    # Create bench executable
    add_executable(${BENCH_TARGET}
        ${BENCH_SOURCES}
        ${PRIME_SOURCES}
    )
    
    # Set prime-specific definition for both targets
    target_compile_definitions(${TEST_TARGET} PRIVATE PRIME_ID=${PRIME_DEF})
    target_compile_definitions(${BENCH_TARGET} PRIVATE PRIME_ID=${PRIME_DEF})
    
    # Add include directories for both targets
    target_include_directories(${TEST_TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/primes/${DIR_NAME}
    )
    target_include_directories(${BENCH_TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/primes/${DIR_NAME}
    )
    
    # Add test to test suite
    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
    
endforeach()

# Enable testing
enable_testing()

# Print build configuration
message(STATUS "Build configuration:")
message(STATUS "  Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  Flags: ${CMAKE_C_FLAGS}")
message(STATUS "  ARM optimizations: ${USE_ARM_OPTIMIZATIONS}")
