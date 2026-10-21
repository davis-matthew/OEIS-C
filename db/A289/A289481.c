/*
 * A289481  Number A(n,k) of Dyck paths of semilength k*n and height n; square
 *          array A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A289481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) counts Dyck paths of semilength k*n and height exactly n
 *            where n and k are obtained from the index n by reading the
 *            square array A(n,k) along antidiagonals. The implementation
 *            computes A(row,col) by dynamic programming counting paths of
 *            length 2*m (m = row*col) that never go below 0 and have...
 * Range:     u8 n <= 18, u16 n <= 38, u32 n <= 58, u64 n <= 95, u128 n <= 160,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"

static idx_t antidiagonal(idx_t n, idx_t *i, idx_t *j)
{
    /* Find largest d such that d*(d+1)/2 <= n. */
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    *i = n - (low * (low + 1) / 2);
    *j = low - *i;
    return low;
}

static seq_t catalan(idx_t m)
{
    if (m == 0)
        return (seq_t)1;
    seq_t c = binom(2 * m, m);
    seq_t div = (seq_t)(m + 1);
    return c / div;
}

static seq_t bounded_dyck(idx_t m, idx_t maxh)
{
    /* Count Dyck paths of semilength m with max height <= maxh. */
    if (maxh >= m)
        return catalan(m);
    if (m == 0)
        return (seq_t)1;   /* empty path reachable for any maxh >= 0 */
    if (maxh == 0)
        return (seq_t)0;   /* cannot stay above 0 when m>0 */

    size_t H = (size_t)(maxh + 1);
    seq_t *dp_cur = (seq_t *)calloc(H, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(H, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        /* fallback: return 0 on allocation failure */
        free(dp_cur);
        free(dp_next);
        return (seq_t)0;
    }

    dp_cur[0] = (seq_t)1;
    for (idx_t step = 0; step < 2 * m; ++step) {
        /* zero dp_next */
        for (size_t h = 0; h < H; ++h)
            dp_next[h] = (seq_t)0;
        for (size_t h = 0; h < H; ++h) {
            seq_t val = dp_cur[h];
            if (!val)
                continue;
            if (h + 1 < H)
                dp_next[h + 1] += val;
            if (h > 0)
                dp_next[h - 1] += val;
        }
        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp_cur[0];
    free(dp_cur);
    free(dp_next);
    return result;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal(n, &i, &j);   /* i = row (height), j = column (factor) */

    if (j == 0) {              /* semilength m = 0 */
        return (i == 0) ? (seq_t)1 : (seq_t)0;
    }

    idx_t m = i * j;           /* semilength = row * column */

    seq_t fi = bounded_dyck(m, i);
    seq_t fi_minus1 = (i == 0) ? (seq_t)0 : bounded_dyck(m, i - 1);

    return fi - fi_minus1;
}
