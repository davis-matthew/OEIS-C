/*
 * A396869  a(n) = 4*q - floor(2*sqrt(q))^2, where q = A246655(n).
 * https://oeis.org/A396869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*q - floor(2*sqrt(q))^2, where q = A246655(n)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

static seq_t *prime_powers = NULL;
static size_t pp_count = 0;
static bool pp_initialized = false;

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b) {
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Build, sort and store the prime‑power sequence A246655 up to a limit
   that is guaranteed to contain at least 10000 elements. */
static void init_prime_powers(void) {
    if (pp_initialized) return;

    /* 500 000 is more than enough to contain >10 000 prime powers
       (≈38 000 primes ≤ 500 000 plus higher powers). */
    const size_t limit = 500000;

    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) exit(1);
    memset(is_prime, 1, limit + 1);
    is_prime[0] = 0;
    is_prime[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Allocate array large enough for all prime powers up to the limit. */
    size_t cap = limit;               /* safe upper bound */
    prime_powers = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!prime_powers) exit(1);
    pp_count = 0;

    for (size_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;
        unsigned long long val = p;
        while (val <= limit) {
            prime_powers[pp_count++] = (seq_t)val;
            unsigned long long next = val * (unsigned long long)p;
            if (next > limit) break;
            val = next;
        }
    }
    free(is_prime);

    qsort(prime_powers, pp_count, sizeof(seq_t), cmp_seq);
    pp_initialized = true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    init_prime_powers();
    seq_t q = prime_powers[n - 1];
    seq_t t = floor_sqrt((seq_t)4 * q);
    seq_t ans = (seq_t)4 * q - t * t;
    return ans;
}
