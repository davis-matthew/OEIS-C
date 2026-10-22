/*
 * A393150  a(n) is chosen such that the root mean square of all terms so far
 *          is closest to n by absolute difference.
 * https://oeis.org/A393150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(sqrt(n^3 - Sum_{i=1..n-1} a(i)^2))
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the terms of OEIS sequence A393150
   using a simple memoized O(n) algorithm.               */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *a_cache = NULL;   /* a_cache[1] = a(1), a_cache[0] unused */
static seq_t *cum_cache = NULL; /* cum_cache[i] = Σ_{k=1..i} a(k)^2, cum_cache[0] = 0 */
static idx_t max_computed = 0;   /* largest n for which a_cache[n] is defined */
static size_t cache_cap = 0;

/* Ensure that the cache contains all terms up to and including n. */
static void ensure_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    /* Grow the cache: start with a small size and double until it fits. */
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_a  = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    seq_t *new_cum = (seq_t *)realloc(cum_cache, new_cap * sizeof(seq_t));
    a_cache = new_a;
    cum_cache = new_cum;
    cache_cap = new_cap;

    /* Initialize the zero element if we are starting from scratch. */
    if (max_computed == 0) {
        a_cache[0]   = (seq_t)0; /* unused but defined */
        cum_cache[0] = (seq_t)0; /* sum of squares up to index 0 */
    }

    /* Fill the cache from max_computed+1 up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sn  = (seq_t)i;
        seq_t n3  = sn * sn * sn;         /* n³ */
        seq_t prev_sum = cum_cache[i - 1]; /* Σ_{k=1}^{i-1} a(k)² */
        seq_t x = n3 - prev_sum;           /* argument of sqrt */

        /* The floor sqrt helper returns an integer less than or equal
           to the true sqrt of its argument. */
        seq_t f  = floor_sqrt(x);
        seq_t f_sq = f * f;
        seq_t diff_f = x - f_sq;

        seq_t c  = f + 1;
        seq_t c_sq = c * c;
        seq_t diff_c = c_sq - x;

        seq_t val = (diff_f < diff_c) ? f : c;

        a_cache[i] = val;
        cum_cache[i] = prev_sum + (val * val);
    }

    max_computed = n;
}

/* Public accessor. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0; return 0 to avoid UB. */
        return (seq_t)0;
    }
    ensure_up_to(n);
    return a_cache[n];
}
