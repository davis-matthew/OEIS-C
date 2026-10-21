/*
 * A275459  G.f.: 3F2([4/9, 5/9, 8/9], [2/3, 1], 729 x).
 * https://oeis.org/A275459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275459
   Recurrence:  a(0) = 1
   for n >= 1:  a(n) = 3*(9*n-5)*(9*n-4)*(9*n-1) / (n*n*(3*n-1)) * a(n-1)
   This implementation stores the values in a static cache so that any order
   of access is supported.  The numerators are reduced with the denominators
   using gcd to avoid overflow in the 64‑bit build.  All operations are
   performed on type seq_t; the helper function gcd from ../utils/gcd.h
   (seq_t gcd(seq_t, seq_t)) is used. */

#include <stdlib.h>
#include "../utils/gcd.h"

/* static cache for previously computed terms */
static seq_t *cache = NULL;    /* pointer to array of seq_t */
static idx_t cache_size = 0;   /* number of stored elements (cache_size = n+1 if a(n) stored) */

/* compute the next term a(n) from a(n‑1) using the recurrence */
static seq_t compute_next(idx_t n, seq_t prev_val)
{
    seq_t N = (seq_t)n;

    /* numerator factors: 3, (9n-5), (9n-4), (9n-1) */
    seq_t num[4] = { (seq_t)3,
                     9 * N - (seq_t)5,
                     9 * N - (seq_t)4,
                     9 * N - (seq_t)1 };

    /* denominator factors: n, n, (3n-1) */
    seq_t den[3] = { N,
                     N,
                     3 * N - (seq_t)1 };

    /* reduce common factors between numerator and denominator */
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            seq_t g = gcd(num[i], den[j]);   /* gcd from utils/gcd.h */
            if (g > (seq_t)1) {
                num[i] /= g;
                den[j] /= g;
            }
        }
    }

    /* divide the previous value by the remaining denominator factors */
    seq_t result = prev_val;
    for (int j = 0; j < 3; ++j) {
        if (den[j] > (seq_t)1) {
            result /= den[j];
        }
    }

    /* multiply by the remaining numerator factors */
    for (int i = 0; i < 4; ++i) {
        if (num[i] > (seq_t)1) {
            result *= num[i];
        }
    }

    return result;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    /* if already computed, just return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow cache to hold up to index n */
    size_t needed = (size_t)(n + 1);          /* array size needed */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
    if (!tmp) {
        /* allocation failure – return 0 as a sentinel (won't happen in tests) */
        return (seq_t)0;
    }
    cache = tmp;

    /* init first value if this is the first call */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = compute_next(i, cache[i - 1]);
    }
    cache_size = n + 1;

    return cache[n];
}
