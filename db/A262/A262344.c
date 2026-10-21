/*
 * A262344  Centered 11-gonal (or hendecagonal) primes.
 * https://oeis.org/A262344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11*k^2 - 11*k + 2)/2 where k is the n‑th integer for
 *            which this expression is prime
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 18, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found terms and the next k to test */
    static seq_t *cache     = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;
    static idx_t   next_k    = 1;          /* 1‑based integer to test */

    /* initial allocation */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* should not happen */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = ((seq_t)11 * k * k - (seq_t)11 * k + (seq_t)2) / (seq_t)2;

        if (is_prime_big(val)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
