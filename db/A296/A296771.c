/*
 * A296771  Row sums of A050157.
 * https://oeis.org/A296771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 3/2) * binomial(2*n, n) - 2^(2*n-1) for n ≥ 1, a(0)
 *            = 1. This uses the supplied utilities binomial and pow2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t two_n = n << 1;                     /* 2*n */
    seq_t bin   = binomial(two_n, n);         /* C(2n, n) */
    seq_t half  = bin / 2;                    /* exact division: C(2n,n) is even */
    seq_t prod  = half * ((seq_t)two_n + 3);  /* (2n+3)*C(2n,n)/2            */
    seq_t powv  = pow2(two_n - 1);            /* 2^(2n-1)                     */
    return prod - powv;
}
