// BENCHMARKING

#include <time.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "arith.h"
#include "random.h"

#define WARMUP      5000
#define BENCH_LOOPS 15000
#define BATCH       1000  // Operations per timing measurement

// Volatile sink prevents compiler from optimizing away function calls
static volatile digit_t sink;

// Returns nanoseconds using monotonic clock (unaffected by system time changes)
static inline uint64_t get_time_ns(void) {
    struct timespec ts;
#if defined(__linux__)
    // Use MONOTONIC_RAW on Linux (not adjusted by NTP)
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
#else
    // Use MONOTONIC on macOS and other platforms
    clock_gettime(CLOCK_MONOTONIC, &ts);
#endif
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

// Format number with thousands separators for readability
static const char* format_number(uint64_t num) {
    static char buffer[32], formatted[32];
    snprintf(buffer, sizeof(buffer), "%" PRIu64, num);
    int len = strlen(buffer), j = 0;
    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) formatted[j++] = '\'';
        formatted[j++] = buffer[i];
    }
    formatted[j] = '\0';
    return formatted;
}


static void __attribute__((noinline)) bench_fun(int sel, f_elm_t *t0, f_elm_t* t1, unsigned char* s, int i){
    switch (sel) {
        case 0: f_red(t0[i]); sink = t0[i][0]; break;
        case 1: f_add(t0[i], t1[i], t0[i]); sink = t0[i][0]; break;
        case 2: f_neg(t0[i], t0[i]); sink = t0[i][0]; break;
        case 3: f_sub(t0[i], t1[i], t0[i]); sink = t0[i][0]; break;
        case 4: f_mul(t0[i], t1[i], t0[i]); sink = t0[i][0]; break;
        case 5: f_leg(t0[i], &s[i]); sink = s[i]; break;
        case 6: f_inv(t0[i], t0[i]); sink = t0[i][0]; break;
        case 7: f_sqrt(t0[i], t0[i]); sink = t0[i][0]; break;
        default: break;
    }
}


int main(void){
    const char *function_names[] = {"f_red", "f_add", "f_neg", "f_sub", "f_mul", "f_leg", "f_inv", "f_sqrt"};
    f_elm_t *t0 = NULL, *t1 = NULL;
    unsigned char *s = NULL;
    int result = 0;

    t0 = malloc(BENCH_LOOPS * sizeof(f_elm_t));
    t1 = malloc(BENCH_LOOPS * sizeof(f_elm_t));
    s = calloc(BENCH_LOOPS, sizeof(unsigned char));

    if (!t0 || !t1 || !s) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        result = 1;
        goto cleanup;
    }

    // Generate random field values
    for(int i = 0; i < BENCH_LOOPS; i++){
        f_rand(t0[i]);
        f_rand(t1[i]);
    }

    // WARMUP
    for(int i = 0; i < WARMUP; i++){
        f_mul(t0[i], t1[i], t1[i]);
    }

    // BENCHMARKING
    const char *prime_names[] = {"p64_0", "p64_1", "p128_0", "p128_1", "p192_0", "p192_1", "p256_0", "p256_1", "p512_0", "p512_1"};
    printf("Benchmarking %s\n\n", prime_names[PRIME_ID]);

    for(int sel = 0; sel < 8; sel++){
        uint64_t total_ns = 0;
        
        for(int i = 0; i < BENCH_LOOPS; i += BATCH){
            int batch_end = (i + BATCH <= BENCH_LOOPS) ? i + BATCH : BENCH_LOOPS;
            uint64_t time_start = get_time_ns();
            for(int j = i; j < batch_end; j++){
                bench_fun(sel, t0, t1, s, j);
            }
            uint64_t time_end = get_time_ns();
            total_ns += (time_end - time_start);
        }
        
        uint64_t avg_ns = total_ns / BENCH_LOOPS;
        printf("%-8s %9s ns/op\n", function_names[sel], format_number(avg_ns));
    }
    printf("\n");

cleanup:
    free(t0);
    free(t1);
    free(s);
    return result;
}
