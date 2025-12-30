#include "../../arith.h"

extern void f_red_asm(f_elm_t a);
extern void f_add_asm(const digit_t* a, const digit_t* b, digit_t* c);
extern void f_neg_asm(const f_elm_t a, f_elm_t b);
extern void f_sub_asm(const f_elm_t a, const f_elm_t b, f_elm_t c);
extern void mp_mul_asm(const digit_t* a, const digit_t* b, digit_t* c);
extern void mont_redc_asm(const digit_t* a, digit_t* c);
extern void f_mul_asm(const digit_t* a, const digit_t* b, digit_t* c);
extern void f_leg_asm(const digit_t* a, unsigned char* b);
extern void f_inv_asm(const digit_t* a, digit_t* b);
extern void f_sqrt_asm(const digit_t* a, digit_t* b);

// Reduction modulo p
inline void f_red(f_elm_t a) {
    f_red_asm(a); }

// Generate a random field element
void f_rand(f_elm_t a)
{
    randombytes((unsigned char *)a, sizeof(digit_t) * WORDS_FIELD);
    f_red(a); // Not uniformly random, can use rejection sampling to fix, thought it's pretty close to uniform
}

// Addition of two field elements
inline void f_add(const f_elm_t a, const f_elm_t b, f_elm_t c) {
    f_add_asm(a, b, c); }

// Negation of a field element
inline void f_neg(const f_elm_t a, f_elm_t b) {
    f_neg_asm(a, b); }

// Subtraction of two field elements
inline void f_sub(const f_elm_t a, const f_elm_t b, f_elm_t c) {
    f_sub_asm(a, b, c); }

// Multiplication of two multiprecision words (without reduction)
inline void mp_mul(const digit_t *a, const digit_t *b, digit_t *c) {
    mp_mul_asm(a, b, c); }

// Montgomery form reduction after multiplication
inline void mont_redc(const digit_t *a, digit_t *c) {
    mont_redc_asm(a, c); }

// Multiplication of field elements
inline void f_mul(const f_elm_t a, const f_elm_t b, f_elm_t c) {
    f_mul_asm(a, b, c); }



// Convert a number from value to Montgomery form  (a -> aR)
void to_mont(const digit_t *a, f_elm_t b)
{
    f_mul(a, R2, b);
}


// Convert a number from Montgomery form into value (aR -> a)
void from_mont(const f_elm_t a, digit_t *b)
{
    digit_t t0[2 * WORDS_FIELD] = {0};
    f_copy(a, t0);
    mont_redc(t0, b);
}

// Multiplicative inverse of a field element
void f_inv(const f_elm_t a, f_elm_t b){
    f_inv_asm(a, b); }

// Legendre symbol of a field element
void f_leg(const f_elm_t a, unsigned char *b){
    f_leg_asm(a, b); }

// Square root of a field element
void f_sqrt(const f_elm_t a, f_elm_t b){
    f_sqrt_asm(a, b); }


