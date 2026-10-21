/*
 * A250272  Number of length n+1 0..3 arrays with the sum of the squares of
 *          adjacent differences multiplied by some arrangement of +-1 equal
 *          to zero.
 * https://oeis.org/A250272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the empirical recurrence * a(n) = 22·a(n-1) -
 *            221·a(n-2) + 1348·a(n-3) - 5594·a(n-4) + 16756·a(n-5) * -
 *            37474·a(n-6) + 63792·a(n-7) - 83421·a(n-8) + 83878·a(n-9) * -
 *            64369·a(n-10) + 37052·a(n-11) - 15496·a(n-12) + 4448·a(n-13) * -
 *            784·a(n-14) + 64·a(n-15) * for all n > 18. * Initia...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Initial terms a(1)..a(18) */
static const seq_t init_vals[18] = {
    (seq_t)4,
    (seq_t)20,
    (seq_t)52,
    (seq_t)240,
    (seq_t)984,
    (seq_t)4412,
    (seq_t)20252,
    (seq_t)91808,
    (seq_t)406748,
    (seq_t)1759740,
    (seq_t)7455484,
    (seq_t)31056840,
    (seq_t)127719296,
    (seq_t)520368940,
    (seq_t)2106440916,
    (seq_t)8489650480,
    (seq_t)34118389900,
    (seq_t)136865613068
};

/* Recurrence coefficients for a(n) when n > 18 */
static const int coeffs[15] = {
    22,   -221,  1348, -5594, 16756,
   -37474, 63792, -83421, 83878, -64369,
    37052, -15496, 4448, -784, 64
};

/* Cache for computed values */
static seq_t *cache = NULL;    /* terms a(1) .. a(len) */
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure cache holds values up to index n (1‑based) */
static void ensure_cache_size(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* grow capacity */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (n > 100) ? n + 100 : 100;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* first time: load the initial 18 values */
    if (cache_len == 0) {
        for (int i = 0; i < 18; ++i)
            cache[i] = init_vals[i];
        cache_len = 18;
    }

    /* compute further values using the recurrence */
    /* maintain a sliding window of the last 15 computed terms */
    seq_t last[15];
    for (int i = 0; i < 15; ++i)
        last[i] = cache[cache_len - 15 + i];  /* a(k) with k = cache_len-14 .. cache_len */

    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t sumPos = 0, sumNeg = 0;
        for (int k = 0; k < 15; ++k) {
            seq_t term = last[14 - k];   /* a(i-1-k) */
            int c = coeffs[k];
            if (c >= 0)
                sumPos += (seq_t)c * term;
            else
                sumNeg += (seq_t)(-c) * term;
        }
        seq_t res = sumPos - sumNeg;

        cache[cache_len++] = res;

        /* shift window and append new term */
        for (int t = 0; t < 14; ++t)
            last[t] = last[t + 1];
        last[14] = res;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined; harmless */

    if (n <= 18)
        return init_vals[n - 1];

    ensure_cache_size(n);
    return cache[n - 1];
}
