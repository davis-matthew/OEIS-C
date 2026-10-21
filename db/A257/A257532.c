/*
 * A257532  Triangle, read by rows, T(n,k)=k/n*Sum_{i=0..n-k}
 *          C(2*n,n-k-i)*C(2*n+i-1,i).
 * https://oeis.org/A257532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with
 *            T(n,k)=k/n*∑_{i=0}^{n-k}C(2n,n-k-i)*C(2n+i-1,i), where r is the
 *            smallest integer with r(r+1)/2 ≥ n (row index), and k=n−r(r−1)/2
 *            is the column within that row (1‑based)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static idx_t
row_for_idx(idx_t n)
{
    /* Incremental while loop to avoid overflow; works for any idx_t. */
    idx_t r = 1;
    idx_t acc = 0;                  /* accumulated elements before row r */
    while (acc + r < n) {
        acc += r;
        ++r;
    }
    return r;                      /* r ≥ 1 for any n ≥ 1 */
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset is 1; undefined for n < 1 */

    idx_t r = row_for_idx(n);
    idx_t prev = r * (r - 1) / 2;   /* elements before row r */
    idx_t k = n - prev;             /* column index (1‑based) */

    idx_t max_i = r - k;            /* i ranges from 0 to r-k */
    seq_t sum = 0;

    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t c1 = binom(2 * r, r - k - i);   /* C(2r, r-k-i) */
        seq_t c2 = binom(2 * r + i - 1, i);   /* C(2r+i-1, i) */
        sum += c1 * c2;
    }

    /* k/r is an exact integer division by definition. */
    return (sum * (seq_t)k) / (seq_t)r;
}
