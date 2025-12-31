#include "parameters.h"

// Prime constants for P64_1 (2^64 - 59)
const digit_t p[WORDS_FIELD]         = {0xFFFFFFFFFFFFFFC5};
const digit_t Mont_one[WORDS_FIELD]  = {0x000000000000003B};
const digit_t R2[WORDS_FIELD]        = {0x0000000000000D99};
const digit_t iR[WORDS_FIELD]        = {0xCBEEA4E1A08AD8C4};
const digit_t pp[WORDS_FIELD]        = {0xCBEEA4E1A08AD8F3};
const digit_t ip[WORDS_FIELD]        = {0x34115B1E5F75270D};
const digit_t Zero[WORDS_FIELD]      = {0x0000000000000000};
const digit_t One[WORDS_FIELD]       = {0x0000000000000001};
