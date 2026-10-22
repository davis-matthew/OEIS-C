/*
 * A321423  Expansion of 1/2 * Product_{0 <= i <= j} (1 + x^(i^2 + j^2)).
 * https://oeis.org/A321423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1/2) * product_{0 <= i <= j} (1 +
 *            x^(i^2 + j^2)) (the factor 1/2 removes the term from (i, j) =
 *            (0,0)). a(0) = 1. This function builds the coefficients up to
 *            the requested index and memoises them for subsequent calls
 * Range:     u8 n <= 55, u16 n <= 155, u32 n <= 492, u64 n <= 1711,
 *            u128 n <= 6333, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static memoisation buffers */
static seq_t *dp_table = NULL;      /* coefficient array a[0..size-1] */
static size_t dp_size = 0;          /* current allocated size */

static uint16_t *rep_count = NULL;  /* r(k) = #pairs 0 <= i <= j with i^2 + j^2 = k */
static size_t rep_size = 0;         /* current allocated size for rep_count */

/* recompute tables up to index n */
static void recompute(idx_t n)
{
    const size_t sz = (size_t)n + 1;

    /* allocate / resize representation counter array */
    if (rep_size < sz) {
        rep_count = (uint16_t *)realloc(rep_count, sz * sizeof(uint16_t));
        rep_size = sz;
    }
    /* initialise to zero */
    for (size_t i = 0; i < sz; ++i) rep_count[i] = 0;

    /* count representations i^2 + j^2 = k, 0 <= i <= j, k>0 */
    for (idx_t i = 0; ; ++i) {
        idx_t i2 = i * i;
        if (i2 > n || i2 < i) break;  /* overflow or beyond limits */
        for (idx_t j = i; ; ++j) {
            idx_t j2 = j * j;
            if (j2 > n || j2 < j) break;
            idx_t sum = i2 + j2;
            if (sum > n) break;
            if (sum != 0) rep_count[(size_t)sum] += 1;
        }
    }

    /* allocate / resize coefficient table */
    if (dp_size < sz) {
        dp_table = (seq_t *)realloc(dp_table, sz * sizeof(seq_t));
        dp_size = sz;
    }

    /* build the coefficients up to n, starting from a[0] = 1 */
    for (size_t i = 0; i < sz; ++i) dp_table[i] = 0;
    dp_table[0] = (seq_t)1;

    for (size_t k = 1; k < sz; ++k) {
        uint16_t r = rep_count[k];
        if (r == 0) continue;
        for (uint16_t t = 0; t < r; ++t) {
            for (idx_t e = n; e >= (idx_t)k; --e) {
                dp_table[(size_t)e] += dp_table[(size_t)(e - k)];
            }
        }
    }
}

seq_t a(idx_t n)
{
    if ((size_t)n + 1 > dp_size) {
        recompute(n);
    }
    return dp_table[(size_t)n];
}
