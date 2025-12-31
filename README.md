# Finite Field Arithmetic

A high-performance C library for constant-time finite field arithmetic operations.

## Features

- **Constant-time operations**: All implementations use constant-time algorithms with no branching on secret data
- **Multiple prime fields**: Support for selected 64, 128, 192, 256, and 512-bit prime fields (10 configurations total)
- **Arithmetic operations**: Modular reduction, addition, subtraction, negation, multiplication, inverse, Legendre symbol, square root
- **Dual implementations**: Generic C and optimized ARM64 assembly versions

## Project Structure

```
Finite-field-arithmetic/
├── include/           # Public API headers
│   ├── arith.h       # Field arithmetic operations
│   ├── parameters.h  # Prime field parameters
│   └── random.h      # Random number generation
├── src/              # Implementation
│   ├── arith.c       # Common arithmetic utilities
│   ├── random/       # Cryptographic RNG
│   └── primes/       # Prime-specific implementations
│       ├── p64_0/    # 2^61 - 1 (Mersenne)
│       ├── p64_1/    # 2^64 - 59
│       ├── p128_0/   # 2^127 - 1 (Mersenne)
│       ├── p128_1/   # 2^128 - 173
│       ├── p192_0/   # 2^192 - 237
│       ├── p192_1/   # 2^191 - 19
│       ├── p256_0/   # 2^255 - 19 (Curve25519)
│       ├── p256_1/   # 2^512 - 2^256 + 2^192 - 2^128 - 1
│       ├── p512_0/   # FIPS 186 prime
│       └── p512_1/   # 2^511 - 2^320 + 1
├── tests/            # Test suite
│   └── tests.c       # Comprehensive field operation tests
├── benchmarks/       # Performance benchmarks
│   └── bench.c       # Accurate timing measurements
└── build/            # Build artifacts
```

## Building

### Requirements

- **CMake** 3.15 or higher
- **GCC** or compatible C compiler
- **Make** (optional, for convenience wrapper)

### Quick Start

```bash
# Build everything (all 20 executables: 10 tests + 10 benchmarks)
make

# Build and run specific test
make test256_0

# Build and run specific benchmark
make bench512_1

# Run all tests
make test

# Run all benchmarks
make bench

# Clean build directory
make clean
```

### CMake Direct Usage

```bash
# Configure and build
mkdir build && cd build
cmake ..
cmake --build . -j4

# Run specific executable
./test256_0
./bench256_0
```

### ARM64 Optimizations (Future)

ARM64 assembly implementations are available but not yet integrated into CMake. To enable:

```bash
cmake -DUSE_ARM_OPTIMIZATIONS=ON ..
```

## Prime Field Configurations

The library supports 10 prime field configurations across 5 bit sizes:

### 64-bit Primes

| Config | Prime | Hex Value |
|--------|-------|-----------|
| **p64_0** | 2^61 - 1 | `0x1FFFFFFFFFFFFFFF` |
| **p64_1** | 2^64 - 59 | `0xFFFFFFFFFFFFFFC5` |

### 128-bit Primes

| Config | Prime | Hex Value |
|--------|-------|-----------|
| **p128_0** | 2^127 - 1 | `0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF` |
| **p128_1** | 2^128 - 173 | `0xFFFFFFFFFFFFFFFF FFFFFFFFFFFFFF53` |

### 192-bit Primes

| Config | Prime | Hex Value |
|--------|-------|-----------|
| **p192_0** | 2^192 - 237 | `0xFFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFF13` |
| **p192_1** | 2^191 - 19 | `0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFED` |

### 256-bit Primes

| Config | Prime | Hex Value |
|--------|-------|-----------|
| **p256_0** | 2^255 - 19 | `0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFED` |
| **p256_1** | 2^256 - 2^224 + 2^192 + 2^96 - 1 | `0xFFFFFFFF00000001 0000000000000000 00000000FFFFFFFF FFFFFFFFFFFFFFFF` |

### 512-bit Primes

| Config | Prime | Hex Value |
|--------|-------|-----------|
| **p512_0** | 2^512 - 2^256 + 2^192 - 2^128 - 1 | `0xFFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF 0000000000000000 FFFFFFFFFFFFFFFE FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF` |
| **p512_1** | 2^511 - 2^320 + 1 | `0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000001` |

## Testing

Each test executable runs comprehensive validation of all field operations:

```bash
# Run all tests for all primes
make test

# Run specific prime configuration
make test256_0
make test192_1
```

### Test Output

```
Running tests for p256_0 (256-bit prime)...

f_red        ✔✔✔
f_add        ✔✔✔
f_neg        ✔✔✔
f_sub      ✔✔✔✔✔
f_mul      ✔✔✔✔✔
f_leg        ✔✔✔
f_inv         ✔✔
f_sqr          ✔

✔ All tests passed!
```

Tests validate:
- **Reduction**: Identity, boundary conditions
- **Addition**: Associativity, commutativity, identity
- **Negation**: Additive inverse properties
- **Subtraction**: Inverse of addition, identity
- **Multiplication**: Associativity, commutativity, distributivity, identity
- **Legendre symbol**: Quadratic residue properties
- **Inverse**: Multiplicative inverse correctness
- **Square root**: For quadratic residues

## Benchmarking

Benchmarks use monotonic clocks - `CLOCK_MONOTONIC_RAW` on Linux, `CLOCK_MONOTONIC` on macOS.

```bash
# Run all benchmarks
make bench

# Run specific benchmark
make bench256_0
make bench512_1
```

### Benchmark Output

```
Benchmarking p256_0

f_red           11 ns/op
f_add           17 ns/op
f_neg           12 ns/op
f_sub           19 ns/op
f_mul           61 ns/op
f_leg       13'715 ns/op
f_inv       10'669 ns/op
f_sqrt      11'355 ns/op
```


## API Reference

### Field Element Type

```c
typedef uint64_t digit_t;
typedef digit_t f_elm_t[WORDS_FIELD];  // Field element (size varies by prime)
```

### Core Operations

```c
void f_red(f_elm_t a);                                    // Reduce modulo p
void f_add(const f_elm_t a, const f_elm_t b, f_elm_t c);  // c = a + b mod p
void f_sub(const f_elm_t a, const f_elm_t b, f_elm_t c);  // c = a - b mod p
void f_neg(const f_elm_t a, f_elm_t b);                   // b = -a mod p
void f_mul(const f_elm_t a, const f_elm_t b, f_elm_t c);  // c = a × b mod p
void f_inv(const f_elm_t a, f_elm_t b);                   // b = a^(-1) mod p
void f_sqrt(const f_elm_t a, f_elm_t b);                  // b = sqrt(a) mod p
void f_leg(const f_elm_t a, unsigned char *b);            // b = Legendre symbol of a
```

### Utility Functions

```c
void f_rand(f_elm_t a);                         // Generate random field element
void f_copy(const f_elm_t a, f_elm_t b);        // b = a
int f_eq(const f_elm_t a, const f_elm_t b);     // Test equality
void print_f_elm(const f_elm_t a);              // Print field element
```

### Montgomery Form

```c
void to_mont(const digit_t *a, f_elm_t b);      // Convert to Montgomery form
void from_mont(const f_elm_t a, digit_t *b);    // Convert from Montgomery form
```

## Continuous Integration

GitHub Actions automatically tests all 10 prime configurations on every push:

```yaml
# Each prime configuration tested separately
- make test64_0
- make test64_1
- make test128_0
...
- make test512_1
```

## Development

### Adding New Primes

1. Create directory: `src/primes/pXXX_Y/`
2. Add implementations: `generic/arith_generic.c` and `prime_params.c`
3. Define prime in `include/parameters.h`
4. Update `CMakeLists.txt` with new configuration

### Code Style

- **Naming**: Snake_case for functions, UPPER_CASE for macros
- **Safety**: Always check malloc/calloc, use goto cleanup pattern
- **Performance**: Mark hot paths as `inline`, use `__attribute__((noinline))` for benchmarks
- **Portability**: Use platform detection (`#ifdef`) for OS-specific code

## License

MIT License - Copyright (c) 2024 Novak Kaluđerović

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

