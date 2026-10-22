/*
 * A360118  Number of differences (not all necessarily distinct) between
 *          consecutive divisors of n which are not also divisors of n.
 * https://oeis.org/A360118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9312, u16 n <= 9312, u32 n <= 9312, u64 n <= 9312,
 *            u128 n <= 9312, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A360118 – Number of differences between consecutive divisors of n
 *  that are not divisors of n.
 *  The implementation collects all divisors of n, sorts them, then counts
 *  the gaps that are not divisors of n.  Complexity O(√n log d) where
 *  d is the number of divisors (d ≈ O(n^ε) for any ε>0).
 *  Works for any seq_t (u8, u16, u32, u64 or u128) and idx_t (=uint64_t).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Compare two idx_t values for qsort */
static int cmp_idx(const void *a, const void *b)
{
    idx_t x = *(const idx_t *)a;
    idx_t y = *(const idx_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* number of divisors is at most 2*floor(sqrt(n))+1 */
    idx_t s = (idx_t)floor(sqrt((double)n));
    /* correct potential rounding error */
    while ((s + 1) * (s + 1) <= n) ++s;
    while (s * s > n) --s;

    size_t max_div = 2 * s + 1;
    idx_t *divs = (idx_t *)malloc(sizeof(idx_t) * max_div);
    if (!divs) return (seq_t)0;   /* unlikely on 64‑bit standard alloc */

    size_t dcnt = 0;
    for (idx_t i = 1; i <= s; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            idx_t j = n / i;
            if (j != i) divs[dcnt++] = j;
        }
    }

    qsort(divs, dcnt, sizeof(idx_t), cmp_idx);

    seq_t count = 0;
    for (size_t k = 1; k < dcnt; ++k) {
        idx_t diff = divs[k] - divs[k - 1];
        /* diff > 0 automatically, but keep for clarity */
        if (diff > 0 && n % diff != 0) {
            ++count;
        }
    }

    free(divs);
    return count;
}
