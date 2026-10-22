/*
 * A370234  a(n) = hypergeom([-n, 1 + n, 2 + n, -1 - n], [1/2, 1/2, 1], 1/16).
 *          Row sums of A370233.
 * https://oeis.org/A370234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^{n} binom(n+k,2k) * binom(n+k+1,2k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum is the finite 4F3 hypergeometric value stated in A370234. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 0; k <= n; ++k) {
        idx_t nk = n + k;
        idx_t twok = k << 1;                /* twok = 2*k */
        seq_t term1 = binom(nk, twok);       /* C(n+k, 2k) */
        seq_t term2 = binom(nk + 1, twok);   /* C(n+k+1, 2k) */
        sum += term1 * term2;
    }
    return sum;
}
