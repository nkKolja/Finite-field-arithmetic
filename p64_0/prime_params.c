#include "parameters.h"

// Prime constants for P64_0 (2^61 - 1, pseudo-Mersenne)
const digit_t p[WORDS_FIELD]         = {0x1FFFFFFFFFFFFFFF}; // Field order p
const digit_t Mont_one[WORDS_FIELD]  = {0x0000000000000008}; // R  =  2^{NBITS_PRIME} (mod p)
const digit_t R2[WORDS_FIELD]        = {0x0000000000000040}; // R2 = (2^{NBITS_PRIME})^2 (mod p)
const digit_t iR[WORDS_FIELD]        = {0x4000000000000000}; // iR =  R^(-1) (mod p)
const digit_t pp[WORDS_FIELD]        = {0x2000000000000001}; // pp = -p^(-1) mod R
const digit_t ip[WORDS_FIELD]        = {0xDFFFFFFFFFFFFFFF}; // ip =  p^(-1) mod R    
const digit_t Zero[WORDS_FIELD]      = {0x0000000000000000}; // Zero = 0
const digit_t One[WORDS_FIELD]       = {0x0000000000000001}; // One = 1
