/*
 * A319846  Irregular triangle read by rows in which row n lists the odd
 *          divisors of n in decreasing order together with the even divisors
 *          of n in decreasing order.
 * https://oeis.org/A319846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A319846: the n‑th entry of the irregular triangle where
 * row k lists the odd divisors of k in decreasing order followed by
 * the even divisors of k in decreasing order. */

#include <stddef.h>
#include <stdlib.h>

static int cmp_desc(const void *p1, const void *p2)
{
    const idx_t *a = (const idx_t *)p1;
    const idx_t *b = (const idx_t *)p2;
    if (*a < *b) return 1;
    if (*a > *b) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1, but safe */

    idx_t cum = 0; /* number of elements already counted */

    for (idx_t k = 1; ; ++k) {
        /* arrays to hold odd and even divisors of k */
        static idx_t odd[256];
        static idx_t even[256];
        size_t odd_cnt = 0, even_cnt = 0;

        /* enumerate divisors of k */
        for (idx_t i = 1; i * i <= k; ++i) {
            if (k % i == 0) {
                idx_t d1 = i;
                idx_t d2 = k / i;

                if (d1 & 1ULL) odd[odd_cnt++] = d1;
                else even[even_cnt++] = d1;

                if (d1 != d2) {
                    if (d2 & 1ULL) odd[odd_cnt++] = d2;
                    else even[even_cnt++] = d2;
                }
            }
        }

        /* sort odd and even divisors in decreasing order */
        if (odd_cnt > 1) qsort(odd, odd_cnt, sizeof(idx_t), cmp_desc);
        if (even_cnt > 1) qsort(even, even_cnt, sizeof(idx_t), cmp_desc);

        size_t total = odd_cnt + even_cnt;
        if (cum + total >= n) {
            idx_t within_row = n - cum - 1; /* zero‑based index within this row */
            if (within_row < odd_cnt)
                return (seq_t)odd[within_row];
            else
                return (seq_t)even[within_row - odd_cnt];
        }

        cum += total;
    }
}
