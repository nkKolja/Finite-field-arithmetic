#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>
#include <stdio.h>

// Prime identifiers
#define P64_0   0   // 2^61 - 1 (pseudo-Mersenne)
#define P64_1   1   // 2^64 - 59
#define P128_0  2   // 2^127 - 1 (pseudo-Mersenne)
#define P128_1  3   // 2^128 - 173
#define P192_0  4   // 2^192 - 237
#define P192_1  5   // 2^191 - 19 (pseudo-Mersenne)
#define P256_0  6   // 2^255 - 19 (Curve25519 prime, pseudo-Mersenne)
#define P256_1  7   // 2^256 - 2^224 + 2^192 + 2^96 - 1 (NIST P-256)
#define P512_0  8   // 2^511 - 1 (FIPS 186 prime)
#define P512_1  9   // 2^255 * 19 + 1 (large safe prime)

// PRIME_ID is defined by CMake at compile time via -DPRIME_ID=<value>
// Provide a default for IDE IntelliSense (will be overridden at build time)
#ifndef PRIME_ID
    #define PRIME_ID P256_0  // Default for IDE only
#endif

// Architecture parameters
#define RADIX           64
#define LOG2RADIX       6  
typedef uint64_t        digit_t;        // Unsigned 64-bit digit
typedef uint32_t        hdigit_t;       // Unsigned 32-bit digit
typedef unsigned uint128_t __attribute__((mode(TI)));

// Derive field size from PRIME_ID
#if (PRIME_ID == P64_0 || PRIME_ID == P64_1)
    #define NBITS_FIELD     64
    #define NBYTES_FIELD    8
    #define WORDS_FIELD     1
#elif (PRIME_ID == P128_0 || PRIME_ID == P128_1)
    #define NBITS_FIELD     128
    #define NBYTES_FIELD    16
    #define WORDS_FIELD     2
#elif (PRIME_ID == P192_0 || PRIME_ID == P192_1)
    #define NBITS_FIELD     192
    #define NBYTES_FIELD    24
    #define WORDS_FIELD     3
#elif (PRIME_ID == P256_0 || PRIME_ID == P256_1)
    #define NBITS_FIELD     256
    #define NBYTES_FIELD    32
    #define WORDS_FIELD     4
#elif (PRIME_ID == P512_0 || PRIME_ID == P512_1)
    #define NBITS_FIELD     512
    #define NBYTES_FIELD    64
    #define WORDS_FIELD     8
#else
    #error "Unsupported PRIME_ID"
#endif

typedef digit_t f_elm_t[WORDS_FIELD];

// Prime-specific constants (defined in each p*_*/prime_params.c)
extern const digit_t p[WORDS_FIELD];         // Field order p
extern const digit_t Mont_one[WORDS_FIELD];  // R  = 2^{NBITS_PRIME} (mod p)
extern const digit_t R2[WORDS_FIELD];        // R2 = (2^{NBITS_PRIME})^2 (mod p)
extern const digit_t iR[WORDS_FIELD];        // iR = R^(-1) (mod p)
extern const digit_t pp[WORDS_FIELD];        // pp = -p^(-1) mod R
extern const digit_t ip[WORDS_FIELD];        // ip = p^(-1) mod R
extern const digit_t Zero[WORDS_FIELD];      // Zero
extern const digit_t One[WORDS_FIELD];       // One


#endif
