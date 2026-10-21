/*
 * A298668  Number T(n,k) of set partitions of [n] into k blocks such that the
 *          absolute difference between least elements of consecutive blocks
 *          is always > 1; triangle T(n,k), n>=0, 0<=k<=ceiling(n/2), read by
 *          rows.
 * https://oeis.org/A298668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th entry of triangle T(n,k), read row‑by‑row. For
 *            k=0: T(n,0)= (n==0 ? 1 : 0). For k>0: T(n,k)= Σ_{j=0}^{k-1}
 *            (-1)^j * C(k-1,j) * (k-j)^(n-k)
 * Range:     u8 n <= 31, u16 n <= 58, u32 n <= 113, u64 n <= 261,
 *            u128 n <= 635, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* find row and column */
    idx_t row = 0;
    idx_t rem = n;
    for (;; ++row) {
        /* length of row = ceil(row/2)+1 = (row+3)/2 */
        idx_t len = (row + 3) / 2;
        if (rem < len) break;
        rem -= len;
    }
    idx_t col = rem;          /* 0 <= col < len(row) */

    /* k==0 special case */
    if (col == 0)
        return (row == 0) ? (seq_t)1 : (seq_t)0;

    idx_t k = col;
    idx_t exp = row - k;      /* exponent = n - k */

    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    seq_t binom = 1;          /* C(k-1,0) */

    for (idx_t j = 0; j < k; ++j) {
        seq_t base   = (seq_t)(k - j);
        seq_t pow_val = big_pow(base, (seq_t)exp);
        seq_t term = binom * pow_val;

        if ((j & 1) == 0)
            sum_even += term;
        else
            sum_odd  += term;

        /* update binomial for next j: C(k-1,j+1)=C(k-1,j)*(k-1-j)/(j+1) */
        if (j + 1 < k) {
            seq_t mult = (seq_t)(k - 1 - j);
            binom = (binom * mult) / (j + 1);
        }
    }

    return sum_even - sum_odd;   /* non‑negative result */
}
