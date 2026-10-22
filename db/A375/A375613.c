/*
 * A375613  Triangle read by rows: T(n, k) = n! * 4^k * hypergeom([-k], [-n],
 *          1/4).
 * https://oeis.org/A375613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where (r, k) are the unique integers with 0 ≤ k ≤
 *            r and n = r*(r+1)/2 + k. T(r, k) = Σ_{j=0..k} 4^{k-j} * C(k, j)
 *            * (r-j)!
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    /* Compute k = n - r*(r+1)/2 */
    idx_t k = n - r * (r + 1) / 2;

    /* f = r! */
    seq_t f = 1;
    for (idx_t i = 1; i <= r; ++i) f *= (seq_t)i;

    /* power4k = 4^k */
    seq_t power4k = 1;
    for (idx_t i = 0; i < k; ++i) power4k *= 4;

    seq_t sum = 0;
    seq_t binom = 1;          /* C(k, 0) */
    seq_t fac_cur = f;        /* (r - j)! */

    for (idx_t j = 0; j <= k; ++j) {
        sum += power4k * binom * fac_cur;
        if (j < k) {
            fac_cur = fac_cur / (seq_t)(r - j);     /* (r - j - 1)! */
            power4k = power4k / 4;                  /* 4^(k - j - 1) */
            binom = binom * (seq_t)(k - j) /
                    (seq_t)(j + 1);                 /* C(k, j+1) */
        }
    }

    return sum;
}
