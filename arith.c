#include "arith.h"



// Print a field element
void print_f_elm(const f_elm_t a)
{
    printf("0x");
    for (int i = WORDS_FIELD * sizeof(digit_t) - 1; i >= 0; i--)
        printf("%02X", ((uint8_t *)(a))[i]);
    printf("\n");
}

// Print a double size mp integer
void print_mp_elm(const digit_t *a, const int nwords)
{
    printf("0x");
    for (int i = nwords * sizeof(digit_t) - 1; i >= 0; i--)
        printf("%02X", ((uint8_t *)(a))[i]);
    printf("\n");
}

// Print out an array of nbytes bytes as hex
void print_hex(const unsigned char *a, const int nbytes)
{
    printf("0x");
    for (int i = 0; i < nbytes; i++)
        printf("%02X", ((uint8_t *)(a))[i]);
    printf("\n");
}





// Compare two field elements for equality, 0 if equal, 1 otherwise
uint8_t f_eq(const f_elm_t a, const f_elm_t b)
{
    uint8_t t = 0;
    for(unsigned int i = 0; i < NBYTES_FIELD; i++)
        t |= ((uint8_t *)a)[i] ^ ((uint8_t *)b)[i];

    return ((t | (unsigned char)(0-t)) >> 7);
}

// Compare two field elements for equality, 1 if not equal, 0 otherwise
uint8_t f_neq(const f_elm_t a, const f_elm_t b)
{
    uint8_t t = 0;
    for(unsigned int i = 0; i < NBYTES_FIELD; i++)
        t |= ((uint8_t *)a)[i] ^ ((uint8_t *)b)[i];
    
     return (1 - (((t | (0-t)) >> 7) & 0x01));
}

// if cond == 0, c = a; if cond == 1, c = b
void cond_select(const f_elm_t a, const f_elm_t b, f_elm_t c, uint8_t cond)
{
    digit_t mask = 0 - (digit_t)cond;

    for(int i = 0; i < WORDS_FIELD; i++){
            c[i] = (a[i] ^ b[i]) & mask;
            c[i] ^= a[i];
        }
}


// Copy a field element
void f_copy(const f_elm_t a, f_elm_t b)
{
    for (int i = 0; i < WORDS_FIELD; i++)
        b[i] = a[i];
}



