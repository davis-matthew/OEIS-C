/*
 * A309121  a(n) gives the number of primes in the interval I_j = [(j^2 + 3*j
 *          - 2)/2, j*(j + 5)/2] = [A034856(j), A095998(j)], for j >= 1.
 * https://oeis.org/A309121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the primes in I_n = [(n^2+3n-2)/2 , n(n+5)/2]
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   calc_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_limit.h"

/* Prime sieve and prefix construction */
static void build_sieve(idx_t limit,
                        unsigned char **prime_mark_out,
                        uint32_t **prefix_out) {
    /* Allocate arrays */
    unsigned char *prime_mark = (unsigned char *)malloc(limit + 1);
    if (!prime_mark) {
        /* Allocation failure – abort */
        abort();
    }
    uint32_t *prime_prefix = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!prime_prefix) {
        free(prime_mark);
        abort();
    }

    /* Sieve of Eratosthenes */
    prime_mark[0] = prime_mark[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        prime_mark[i] = 1;
    }
    for (idx_t p = 2; (uint64_t)p * p <= limit; ++p) {
        if (prime_mark[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p) {
                prime_mark[mult] = 0;
            }
        }
    }

    /* Prefix of prime counts */
    prime_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        prime_prefix[i] = prime_prefix[i - 1] + prime_mark[i];
    }

    *prime_mark_out = prime_mark;
    *prefix_out     = prime_prefix;
}

/* Static state */
static idx_t  current_n_limit = 0;
static idx_t  current_limit   = 0;
static unsigned char *prime_mark = NULL; /* 0/1 array */
static uint32_t *prime_prefix     = NULL; /* cumulative counts */

/* Main function to compute a(idx_t n) */
seq_t a(idx_t n) {
    if (n == 0) {
        /* Behavior below offset irrelevant */
        return (seq_t)0;
    }

    /* Ensure sieve covers current n */
    if (n > current_n_limit) {
        /* Release previous data */
        if (prime_mark)  free(prime_mark);
        if (prime_prefix) free(prime_prefix);

        current_n_limit = n;
        current_limit   = calc_limit(n);

        build_sieve(current_limit, &prime_mark, &prime_prefix);
    }

    /* Compute bounds of I_n */
    /* L = (n^2 + 3n - 2)/2, R = n(n+5)/2 */
    uint64_t n_u = (uint64_t)n;
    uint64_t L = (n_u * n_u + 3 * n_u - 2) / 2;
    uint64_t R = n_u * (n_u + 5) / 2;

    if (R > current_limit || L > current_limit) {
        /* Should not happen for valid n */
        return (seq_t)0;
    }

    uint32_t count = prime_prefix[R] - (L > 0 ? prime_prefix[L - 1] : 0);
    return (seq_t)count;
}
