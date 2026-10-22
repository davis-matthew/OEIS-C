/*
 * A395542  Numbers whose number of nonzero digits is identical in base 2 and
 *          5.
 * https://oeis.org/A395542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6810, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base5_nonzero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: numbers k such that
   popcount(k) == number of non‑zero base‑5 digits of k. */
#include <stdlib.h>
#include "../utils/base5_nonzero_digits.h"

static inline seq_t popcount_seq(seq_t x)
{
    seq_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* static memoization storage */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;

    /* initialise with the first term (a(1)=0) */
    if (len == 0) {
        /* allocate room for one element */
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;
        len = 1;
    }

    /* grow the cache until it contains at least n elements */
    while (len < n) {
        /* start from the next integer after the last stored number */
        seq_t cand = cache[len - 1] + 1;
        for (;; ++cand) {
            if (popcount_seq(cand) == base5_nonzero_digits(cand)) {
                /* append to cache */
                if ((size_t)len == cap) {
                    cap = cap * 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = cand;
                break;
            }
        }
    }

    return cache[n - 1];
}
