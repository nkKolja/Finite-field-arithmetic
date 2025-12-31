#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "arith.h"
#include "random.h"

#define TEST_LOOPS      256
#define MAX(a,b)    (((a)>(b))?(a):(b))

#define RED_TESTS   3
#define ADD_TESTS   3
#define NEG_TESTS   3
#define SUB_TESTS   5
#define MUL_TESTS   5
#define LEG_TESTS   3
#define INV_TESTS   2
#define SQRT_TESTS  1
#define NUM_TESTS   8

#define TESTS_PAD   MAX(MAX(MAX(RED_TESTS, ADD_TESTS), MAX(NEG_TESTS, SUB_TESTS)), MAX(MAX(MUL_TESTS, LEG_TESTS), MAX(INV_TESTS, SQRT_TESTS)))
#define TESTS_LEN(x) (  (x) == 0 ? RED_TESTS  : \
                        (x) == 1 ? ADD_TESTS  : \
                        (x) == 2 ? NEG_TESTS  : \
                        (x) == 3 ? SUB_TESTS  : \
                        (x) == 4 ? MUL_TESTS  : \
                        (x) == 5 ? LEG_TESTS  : \
                        (x) == 6 ? INV_TESTS  : \
                        (x) == 7 ? SQRT_TESTS : -1)


#define PASS(x)                 ((x) ? "\033[31m✗\033[0m" : "\033[0;32m✔\033[0m")

const char* pass_check(unsigned char arr[], int n) {
    static char buffer[256] = "";
    buffer[0] = '\0';
    for(int i = 0; i < TESTS_PAD - n; i++)
        strcat(buffer, " ");
    for(int i = 0; i < n; i++)
        strcat(buffer, PASS(arr[i]));
    return buffer;
}


int main(int argc, char* argv[]){
    (void)argv[0];
    (void)argc;

    const char *function_names[] = {"f_red", "f_add", "f_neg", "f_sub", "f_mul", "f_leg", "f_inv", "f_sqr"};
    unsigned char *s = NULL;
    f_elm_t *t0 = NULL, *t1 = NULL, *t2 = NULL;
    unsigned char f;
    f_elm_t s0, s1, s2;
    int result = 0;

    s = calloc(TEST_LOOPS, sizeof(unsigned char));
    t0 = malloc(TEST_LOOPS * sizeof(f_elm_t));
    t1 = malloc(TEST_LOOPS * sizeof(f_elm_t));
    t2 = malloc(TEST_LOOPS * sizeof(f_elm_t));
    
    if (!s || !t0 || !t1 || !t2) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        result = 1;
        goto cleanup;
    }

    // Assign random field values
    for(int i = 0; i < TEST_LOOPS; i++){
        f_rand(t0[i]);
        f_rand(t1[i]);
        f_rand(t2[i]);
    }

    f_elm_t max_val; for(int i = 0; i < WORDS_FIELD; i++) max_val[i] = 0xFFFFFFFFFFFFFFFF;
    f_elm_t Rm1; for(int i = 1; i < WORDS_FIELD; i++) Rm1[i] = Mont_one[i]; Rm1[0] = Mont_one[0] - 1;
    unsigned char tests[NUM_TESTS][256] = {0};

    const char *prime_names[] = {"p64_0", "p64_1", "p128_0", "p128_1", "p192_0", "p192_1", "p256_0", "p256_1", "p512_0", "p512_1"};
    printf("Running tests for %s\n\n", prime_names[PRIME_ID]);

    for(int i = 0; i < TEST_LOOPS; i++){

        // Reduction check
        f_copy(t0[i], s0); f_red(s0);                   // s0 = t0 % p, t0 should already be reduced
        tests[0][0] |= f_eq(s0, t0[i]);

        f_copy(max_val, s0); f_red(s0);                 // s0 = (R - 1) % p
        tests[0][1] |= f_eq(s0, Rm1);

        f_copy(p, s0); f_red(s0);                       // s0 = p % p
        tests[0][2] |= f_eq(s0, Zero);


        // Addition check
        f_add(t0[i], t1[i], s0); f_add(s0, t2[i], s0);  // s0 = (t0 + t1) + t2
        f_add(t1[i], t2[i], s1); f_add(t0[i], s1, s1);  // s1 = t0 + (t1 + t2)
        tests[1][0] |= f_eq(s0, s1);

        f_add(t0[i], t1[i], s0);                        // s0 = t0 + t1
        f_add(t1[i], t0[i], s1);                        // s1 = t1 + t0
        tests[1][1] |= f_eq(s0, s1);

        f_add(t0[i], Zero, s0);                         // s0 = t0 + 0
        tests[1][2] |= f_eq(s0, t0[i]);


        // Negation check
        f_neg(t0[i], s0); f_add(s0, t0[i], s1);         // s0 = -t0 + t0
        tests[2][0] |= f_eq(s1, Zero);

        f_neg(t0[i], s0); f_add(t0[i], s0, s1);         // s0 = t0 - t0
        tests[2][1] |= f_eq(s1, Zero);

        f_neg(Zero, s1);                                // s0 = -0
        tests[2][2] |= f_eq(s1, Zero);


        // Subtraction check
        f_sub(t0[i], t1[i], s0); f_sub(s0, t2[i], s0);  // s0 = (t0 - t1) - t2
        f_sub(t0[i], t2[i], s1); f_sub(s1, t1[i], s1);  // s1 = (t0 - t2) - t1
        f_add(t1[i], t2[i], s2); f_sub(t0[i], s2, s2);  // s2 = t0 - (t1 + t2)
        tests[3][0] |= f_eq(s0, s1);
        tests[3][1] |= f_eq(s0, s2);


        f_sub(t0[i], t1[i], s0);                        // s0 = t0 - t1
        f_sub(t1[i], t0[i], s1); f_neg(s1, s1);         // s1 = -(t1 - t0)
        tests[3][2] |= f_eq(s0, s1);

        f_sub(t0[i], Zero, s0);                         // s0 = t0 - 0
        tests[3][3] |= f_eq(s0, t0[i]);

        f_sub(Zero, t0[i], s0);                         // s0 = 0 - t0
        f_neg(t0[i], s1);                               // s1 = -t0
        tests[3][4] |= f_eq(s0, s1);


        // Multiplication checks
        f_add(t0[i], t1[i], s0); f_mul(s0, t2[i], s0);                       // s0 = (t0 + t1) * t2
        f_mul(t0[i], t2[i], s1); f_mul(t1[i], t2[i], s2); f_add(s1, s2, s1); // s1 = t0 * t2 + t1 * t2
        tests[4][0] |= f_eq(s0, s1);

        f_mul(t0[i], t1[i], s0); f_mul(s0, t2[i], s0);  // s0 = (t0 * t1) * t2
        f_mul(t1[i], t2[i], s1); f_mul(t0[i], s1, s1);  // s1 = t0 * (t1 * t2)
        tests[4][1] |= f_eq(s0, s1);

        f_mul(t0[i], t1[i], s0);                        // s0 = t0 * t1
        f_mul(t1[i], t0[i], s1);                        // s1 = t1 * t0
        tests[4][2] |= f_eq(s0, s1);

        f_mul(t0[i], Mont_one, s0);                     // s0 = t0 * 1
        tests[4][3] |= f_eq(s0, t0[i]);
        
        f_mul(t0[i], Zero, s0);                         // s0 = t0 * 0
        tests[4][4] |= f_eq(s0, Zero);


        // Legendre check
        f_leg(t0[i], s);                                                // s   = f_leg(t0)
        f_mul(t1[i], t1[i], s1); f_mul(s1, t0[i], s1); f_leg(s1, s+1);  // s+1 = f_leg(t0 * t1 * t1)
        if((f_eq(t0[i], Zero)) && (f_eq(t1[i], Zero)))// Ignore input t0 = 0 or t1 = 0
        tests[5][0] |= s[0] ^ s[1];

        f_leg(t0[i], s);                                // s   = f_leg(t0)
        f_leg(t1[i], s+1);                              // s+1 = f_leg(t1)
        f_mul(t0[i], t1[i], s0); f_leg(s0, s+2);        // s+2 = f_leg(t0 * t1)
        if((f_eq(t0[i], Zero)) && (f_eq(t1[i], Zero)))// Ignore input t0 = 0 or t1 = 0
        tests[5][1] |= s[0] ^ s[1] ^ s[2];

        if(i == 0) f_leg(t0[i], &f);
        tests[5][2] |= (f ^ s[0]) |(f ^ s[1]) | (f ^ s[2]); // They are not all equal (to the first value)

        if(i == TEST_LOOPS - 1) tests[5][2] = !tests[5][2]; // If they were all equal result would be 0.


        // Inverse check
        f_inv(t0[i], s0);                               // s0 = t0^(-1)
        f_mul(s0, t0[i], s0);                           // s0 = s0 * s0^(-1)
        if(f_eq(t0[i], Zero))     // Ignore input t0 = 0
        tests[6][0] |= f_eq(s0, Mont_one);
        

        f_inv(t0[i], s0);                               // s0 = t0^(-1)
        f_inv(s0, s1);                                  // s1 = s0^(-1) = t0
        tests[6][1] |= f_eq(s1, t0[i]);


        // Square root test
        f_sqrt(t0[i], s0);                           // s0 = sqrt(t0)
        f_mul(s0, s0, s0);                           // s0 = s0^2 = t0
        f_leg(t0[i], s);
        if(!*s)                                      // Test if t0 is a square
        tests[7][0] |= f_eq(t0[i], s0);

    }

    // Print results
    for(int j = 0; j < NUM_TESTS; j++){
        printf("%s   %s\n", function_names[j], pass_check(tests[j], TESTS_LEN(j)));
    }
    printf("\n");

    for(int i = 0; i < NUM_TESTS; i++)
        for(int j = 0; j < TESTS_LEN(i); j++)
            result |= tests[i][j];

    if(result == 0)
        printf("✔ All tests passed!\n\n");
    else
        printf("✗ Some tests failed!\n\n");

cleanup:
    free(t0);
    free(t1);
    free(t2);
    free(s);
    return result;
}
