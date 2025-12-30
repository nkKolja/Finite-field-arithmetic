// BENCHMARKING

#include <time.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "arith.h"
#include "bench.h"


// Returns nanoseconds on the clock
uint64_t cpucycles(void){
    struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    return (int64_t)(time.tv_sec*1e9 + time.tv_nsec);
}


const char* print_num(double a){
    static char t[256], buffer[256];
    memset(t, 0, sizeof(t)); 
    int len;

    sprintf(t, "%" PRIu64, (uint64_t)a);
    len = strlen(t);

    int j = 0;
    for(int i = 0; i < len; i++){
        if( i > 0 && (len - i )% 3 == 0)
            buffer[j++] = '\'';
        buffer[j++] = t[i];
    }
    buffer[j] = '\0';

    return buffer;
}


static inline void bench_fun(int function_selector, f_elm_t *t0, f_elm_t* t1, unsigned char* s, int i){
    switch (function_selector) {
        case 0:
            f_red(t0[i]);
            break;
        case 1:
            f_add(t0[i], t1[i], t0[i]);
            break;
        case 2:
            f_neg(t0[i], t0[i]);
            break;
        case 3:
            f_sub(t0[i], t1[i], t0[i]);
            break;
        case 4:
            f_mul(t0[i], t1[i], t0[i]);
            break;
        case 5:
            f_leg(t0[i], &s[i]);
            break;
        case 6:
            f_inv(t0[i], t0[i]);
            break;
        case 7:
            f_sqrt(t0[i], t0[i]);
            break;
        default:
            break;
    }
}


int main(int argc, char* argv[]){
    (void)argv[0];
    (void)argc;

    const char *function_names[] = {"f_red", "f_add", "f_neg", "f_sub", "f_mul", "f_leg", "f_inv", "f_sqr"};
    int function_selector;
    unsigned char *s = NULL;
    f_elm_t *t0 = NULL, *t1 = NULL;
    int result = 0;

    s = calloc(BENCH_LOOPS, sizeof(unsigned char));
    t0 = malloc(BENCH_LOOPS * sizeof(f_elm_t));
    t1 = malloc(BENCH_LOOPS * sizeof(f_elm_t));

    if (!s || !t0 || !t1) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        result = 1;
        goto cleanup;
    }

    // Assign random field values
    for(int i = 0; i < BENCH_LOOPS; i++){
        f_rand(t0[i]);
        f_rand(t1[i]);
    }

    uint64_t nsecs_pre, nsecs_post, nsecs;

    // WARM UP
    for(int i = 0; i < WARMUP; i++){
        f_mul(t0[i], t1[i], t1[i]);
    }

    // BENCHMARKING
    const char *prime_names[] = {"p64_0", "p64_1", "p128_0", "p128_1", "p192_0", "p192_1", "p256_0", "p256_1", "p512_0", "p512_1"};
    printf("Benchmarking %s\n\n", prime_names[PRIME_ID]);

    for(int j = 0; j < 8; j++){
        function_selector = j;
        nsecs_pre = 0, nsecs_post = 0, nsecs = 0;

        for(int i = 0; i < BENCH_LOOPS; ){
            nsecs_pre = cpucycles();
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            bench_fun(function_selector, t0, t1, s, i); i++;
            nsecs_post = cpucycles();
            nsecs += (nsecs_post-nsecs_pre);
        }
        printf("%s runs in ......................... %9s ns\n", function_names[function_selector], print_num((double)(nsecs/BENCH_LOOPS)));
    }
    printf("\n");

cleanup:
    free(t0);
    free(t1);
    free(s);
    return result;
}
