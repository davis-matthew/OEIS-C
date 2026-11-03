/*
 * A270835  a(n) = smallest k such that A004431(n) +/- k are both positive
 *          squares.
 * https://oeis.org/A270835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k > 0 such that A004431(n) ± k are both positive
 *            squares. A004431(n) is the n-th integer that can be written as
 *            the sum of two distinct positive squares (a^2 + b^2 with 1 ≤ a <
 *            b). This sequence is generated once and cached
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Comparator for seq_t used in qsort */
static int cmp_seq_t(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Cached values of A004431; the array is filled once on first use. */
static seq_t *a004431_vals  = NULL;   /* sorted unique sums of two distinct squares */
static size_t  a004431_len  = 0;      /* number of elements in cache */

/* Build the cache containing all sums a^2 + b^2 (1 ≤ a < b ≤ MAX_AB)
   that are sorted and unique.  The chosen MAX_AB provides more than enough
   elements for all n ≥ 1 up to at least 10 000. */
static void build_a004431_cache(void)
{
    if (a004431_vals != NULL)   /* already built */
        return;

    const idx_t MAX_AB = 500;           /* generous bound for 10k terms */
    const size_t PAIRED = (size_t)MAX_AB * (MAX_AB - 1) / 2;
    seq_t *temp = mem_new(seq_t, PAIRED);
    if (!temp)
        return;    /* allocation failure; function will return 0 later */

    size_t idx = 0;
    for (idx_t a = 1; a <= MAX_AB; ++a) {
        seq_t a2 = (seq_t)a * a;
        for (idx_t b = a + 1; b <= MAX_AB; ++b) {
            temp[idx++] = a2 + (seq_t)b * b;
        }
    }

    /* Sort and deduplicate */
    mem_qsort(temp, idx, sizeof(seq_t), cmp_seq_t);

    size_t uniq = 0;
    for (size_t i = 0; i < idx; ++i) {
        if (i == 0 || temp[i] != temp[i - 1]) {
            temp[uniq++] = temp[i];
        }
    }

    /* Keep only the unique entries */
    a004431_vals = mem_resize(seq_t, temp, uniq);
    a004431_len  = uniq;
}

/* Return the element of A004431 at position n (1‑based). */
static seq_t get_A004431(idx_t n)
{
    if (a004431_vals == NULL)
        build_a004431_cache();

    /* 1‑based to 0‑based */
    return a004431_vals[n - 1];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* A004431(n) */
    seq_t T = get_A004431(n);

    /* Search for the smallest k such that T ± k are both perfect squares. */
    /* We look for the largest y (y^2 < T) such that 2T - y^2 is a square. */
    seq_t sqrt_T_minus1 = floor_sqrt(T - 1);
    idx_t y_max = (idx_t)sqrt_T_minus1;

    for (idx_t y = y_max; y > 0; --y) {
        seq_t y2 = (seq_t)y * y;
        seq_t val = (seq_t)2 * T - y2;
        seq_t s = floor_sqrt(val);
        if (s * s == val) {
            /* k = T - y^2 */
            return T - y2;
        }
    }

    /* Should never reach here for valid data */
    return 0;
}
