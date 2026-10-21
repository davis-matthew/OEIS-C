/*
 * A275640  Expansion of (1-q)^k/Product_{j=1..k} (1-q^j) for k=6.
 * https://oeis.org/A275640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1−q)^6 / ∏_{j=1..6} (1−q^j) The series satisfies the
 *            linear recurrence a(n) = c_n − Σ_{m=1}^{21} d_m · a(n−m), (n ≥
 *            0) where c_n = (−1)^n·binom(6,n) for n ≤ 6 (0 otherwise) and d_m
 *            are the coefficients of ∏_{j=1..6} (1−q^j)
 * Range:     i8 n <= 31, i16 n <= 619, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Numerator coefficients for c_n, n = 0..6 */
static const seq_t NUM_COEFF[7] = {1, -6, 15, -20, 15, -6, 1};

/* Denominator coefficients for d_m, m = 0..21 */
static const seq_t DENOM[22] = {
    1, -1, -1, 0, 0, 1, 0, 2, 0, -1, -1, -1, -1, 0, 2, 0, 1, 0, 0, -1, -1, 1
};

/* Static cache for computed values */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t len   = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough */
    if (n + 1 > cap) {
        size_t new_cap = cap == 0 ? 1 : cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Compute values from the current length up to n */
    for (idx_t k = len; k <= n; ++k) {
        seq_t val = 0;
        if (k <= 6) val = NUM_COEFF[k];      /* c_k for k ≤ 6, else 0 */

        /* Subtract sum_{m=1}^{min(k,21)} d_m · a(k−m) */
        idx_t maxm = (k < 21 ? k : 21);
        for (idx_t m = 1; m <= maxm; ++m) {
            val -= DENOM[m] * cache[k - m];
        }

        cache[k] = val;
    }
    len = n + 1;    /* up to n inclusive now computed */

    return cache[n];
}
