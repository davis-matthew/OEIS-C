/*
 * A272118  Numbers k such that abs(6*k^2 - 342*k + 4903) is prime.
 * https://oeis.org/A272118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 566, u32 n <= 67, u64 n <= 3874,
 *            u128 n <= 3874, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272118: numbers k such that abs(6*k*k - 342*k + 4903) is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of the first n values found so far. */
    static idx_t count = 0;          /* how many have been stored */
    static uint64_t last_k = 0;      /* next k to test */
    static seq_t *cache = NULL;      /* array of found k's */
    static size_t cap   = 0;         /* allocated capacity of cache */

    /* Allocate cache on first call. */
    if (cache == NULL) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;        /* Out of memory → return 0. */
    }

    /* Produce more terms until we have at least n. */
    while (count < n) {
        /* Compute f(k) = 6*k^2 - 342*k + 4903 in signed 64‑bit. */
        uint64_t k64 = last_k;
        int64_t val = (int64_t)6 * (int64_t)k64 * (int64_t)k64
                    - (int64_t)342 * (int64_t)k64
                    + (int64_t)4903;

        /* Take absolute value. */
        uint64_t abs_val = (val >= 0) ? (uint64_t)val : (uint64_t)-val;

        /* Test primality.  The utility accepts the same type as seq_t. */
        if (is_prime_big((seq_t)abs_val)) {
            /* Grow cache if needed. */
            if (count >= cap) {
                size_t new_cap = cap * 2 + 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* Out of memory → return 0. */
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = (seq_t)last_k;
        }
        ++last_k;
    }

    return cache[n - 1];
}
