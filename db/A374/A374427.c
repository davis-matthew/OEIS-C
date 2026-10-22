/*
 * A374427  Triangle read by rows: T(n, k) = n! * 2^k * hypergeom([-k], [-n],
 *          -1/2).
 * https://oeis.org/A374427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n+1)-th entry of the triangle * T(n, k) =
 *            Σ_{j=0..k} (-1)^j · binomial(k, j) · 2^{k-j} · (n-j)! * where
 *            the index n is decoded as row r and column k * by the triangular
 *            numbering
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* determine row and column of the (n+1)-th entry */
    idx_t row = 0;
    idx_t cum = 0;
    while (cum + (row + 1) <= n) {
        cum += row + 1;
        ++row;
    }
    idx_t k = n - cum;          /* 0 ≤ k ≤ row */

    /* pre‑compute factorials up to 'row' */
    /* row ≤ 141 for n ≤ 10000; allocate a bit buffer */
    seq_t fact[150];
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= row; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* compute T(row, k) by the signed sum formula */
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t comb = binomial(k, j);
        seq_t term = comb * pow2(k - j);
        term *= fact[row - j];
        if ((j & 1) == 0)
            sum_even += term;
        else
            sum_odd += term;
    }
    return sum_even - sum_odd;   /* safe: sum_even ≥ sum_odd for all valid entries */
}
