/*
 * A360992  G.f. satisfies A(x) = 1 + x * (1 - x)^2 * A(x * (1 - x)).
 * https://oeis.org/A360992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 18, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes A360992 by the recurrence
   a(0) = 1; a(n) = Σ_{k=0..n-1} (-1)^k * binomial(n+1-k, k) * a(n-1-k).
   It memoises intermediate values in a static dynamic array so that each
   n is evaluated at most once, giving O(n) time and O(n) memory. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of entries currently stored */

    /* initialise cache with a(0) if needed */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!cache) return 0;          /* allocation failure – undefined behaviour */
        cache[0] = (seq_t)1;           /* a(0) */
        cache_len = 1;
    }

    /* grow cache to accommodate index n if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – undefined behaviour */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing values up to n, starting from the old length */
    for (idx_t i = (idx_t)(cache_len - 1); i <= n; ++i) {
        /* the loop variable 'i' is always > 0 for i == 0 it is not entered */
        if (i == 0) continue;           /* a(0) already known */

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t bin = binom((idx_t)(i + 1 - k), k);
            seq_t term = bin * cache[i - 1 - k];
            if (k & 1)
                term = -term;          /* (-1)^k */
            sum += term;
        }
        cache[i] = sum;
    }

    return cache[n];
}
