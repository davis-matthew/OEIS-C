/*
 * A363419  Square array read by ascending antidiagonals: T(n,k) = 1/n * [x^k]
 *          1/((1 - x)*(1 - x^2))^(n*k) for n, k >= 1.
 * https://oeis.org/A363419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,k) where (i,k) is the pair with the n‑th * element of
 *            the square array read by ascending antidiagonals * with
 *            decreasing row index inside each diagonal. * T(i,k) = 1/i *
 *            Σ_{j=0..⌊k/2⌋} binom(i*k+j-1 , j) * * binom((i+1)k-2j-1 , k-2j)
 *            for i,k ≥ 1 * * The implementation uses the helper f...
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the diagonal sum s such that (s-1)*s/2 > n.
     * The smallest such s satisfies the inequality.
     * For n up to about 10^5, the loop iterates < 300 times. */
    idx_t s = 2;
    for (;; ++s) {
        unsigned __int128 cum = (unsigned __int128)(s - 1) * s / 2;
        if (cum > n) break;
    }

    /* Start index of this diagonal */
    unsigned __int128 start128 = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t start = (idx_t)start128;

    /* Position within diagonal */
    idx_t offset = n - start;

    /* Row (i) and column (k) indices for T(i,k) */
    idx_t i = s - 1 - offset;    /* 1 ≤ i ≤ s-1 */
    idx_t k = 1 + offset;        /* 1 ≤ k ≤ s-1 */

    /* Compute T(i,k) via the stated summation formula */
    idx_t nk     = i * k;
    idx_t maxj   = k / 2;
    seq_t sum = 0;

    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t a1 = binom(nk + j - 1, j);
        seq_t a2 = binom((i + 1) * k - 2 * j - 1, k - 2 * j);
        sum += a1 * a2;
    }

    return sum / (seq_t)i;
}
