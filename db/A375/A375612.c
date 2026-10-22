/*
 * A375612  Triangle read by rows: T(n, k) = n! * 4^k * hypergeom([-k], [-n],
 *          -1/4).
 * https://oeis.org/A375612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = ⌊(−1+√(8n+1))/2⌋ and k = n – r(r+1)/2 ;
 *            T(n, k) = ∑_{j=0}^{k} (−1)^j binom(k, j)·4^{k−j}·(n−j)! This
 *            implements the triangle read by rows from OEIS A375612
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* find row r and column k */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n)
        ++r;
    --r;                          /* r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t k = n - r * (r + 1) / 2;

    /* compute T(r, k) = sum_{j=0..k} (-1)^j binom(k, j) * 4^{k-j} * (r-j)! */
    seq_t sum_pos = 0;   /* even j terms */
    seq_t sum_neg = 0;   /* odd j terms */

    for (idx_t j = 0; j <= k; ++j) {
        seq_t term = binom(k, j);
        term = term * factorial(r - j);
        term = term * pow_uint(4, k - j);
        if ((j & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    return sum_pos - sum_neg;   /* sum_pos ≥ sum_neg for all n, k */
}
