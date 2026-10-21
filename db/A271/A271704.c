/*
 * A271704  Triangle read by rows, T(n,k) = Sum_{j=0..n}
 *          (-1)^(n-j)*C(-j,-n)*L(j,k), L the unsigned Lah numbers A271703,
 *          for n>=0 and 0<=k<=n.
 * https://oeis.org/A271704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows. T(r,k) = Σ_{j=k..r} C(r-1,j-1) *
 *            L(j,k), where L(j,k) = j!/(k!) * C(j-1,k-1) (unsigned Lah
 *            numbers)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Determine row r and column k of the flattened triangle. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Base cases: single element row and zero‑columns. */
    if (r == 0) return (k == 0 ? (seq_t)1 : (seq_t)0);
    if (k == 0) return (seq_t)0;

    seq_t sum = 0;
    seq_t prod = 1;  /* product_{t=k+1}^{j} t , initially 1 for j = k */

    for (idx_t j = k; j <= r; ++j) {
        seq_t c1 = binomial(r - 1, j - 1);
        seq_t c2 = binomial(j - 1, k - 1);  /* equals 1 when j == k */
        seq_t term = c1 * c2 * prod;        /* combine multiplicatively */
        sum += term;
        if (j < r) prod *= (j + 1);         /* prepare product for next j */
    }
    return sum;
}
