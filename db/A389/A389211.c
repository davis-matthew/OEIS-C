/*
 * A389211  Numbers k such that A048146(k)+A162296(k) <= 2*k, where A048146 is
 *          the sum of non-unitary divisors of n, and A162296 is the sum of
 *          divisors of n that have a square factor.
 * https://oeis.org/A389211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that * A048146(k)+A162296(k) ≤
 *            2·k. * Here A048146(k) is the sum of the non‑unitary divisors of
 *            k, * A162296(k) is the sum of the divisors of k that contain a
 *            square factor. * The function returns the k-th such integer
 *            (a(1)=1, a(2)=2, …)
 * Range:     u8 n <= 60, u16 n <= 9548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   condition_holds
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/condition_holds.h"

/* Variants of idx_t and seq_t are provided by the build system. */

/* --------------------------------------------------------------------- */
/*  Helper: test whether the condition holds for a given integer k.      */

/* --------------------------------------------------------------------- */
/*  Main function: a(idx_t n)                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache of the sequence terms. */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0, cache_cap = 0;

    /* Keep track of the next integer to test. */
    static idx_t next_k = 1;

    /* Grow the cache vector when needed. */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < n) {
        /* Test the current k. */
        if (condition_holds(next_k)) {
            /* Add to cache. */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap < 8) ? 8 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
