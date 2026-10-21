/*
 * A288771  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 505", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For odd n: a(n) = 2^n - 1 For even n=2k: let t =
 *            floor(log2(k+1)) m = 2^t - 1 // largest (2^t - 1) ≤ k a(n) =
 *            2^(2k+1) - 2^(2k - m)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* odd n */
    if (n & 1UL)
    {
        seq_t v = pow2(n);     /* 2^n */
        return v - 1;          /* 2^n - 1 */
    }

    /* even n = 2*k */
    idx_t k = n >> 1;

    /* compute t = floor(log2(k+1)) via bit scan */
    idx_t t = 0;
    idx_t tmp = k + 1;
    while (tmp >> 1UL)
    {
        tmp >>= 1UL;
        t++;
    }

    /* m = 2^t - 1 */
    idx_t m = ((idx_t)1 << t) - 1;

    /* exponents for powers of two */
    idx_t exp1 = 2 * k + 1;          /* 2k + 1 */
    idx_t exp2 = 2 * k - m;          /* 2k - m */

    seq_t p1 = pow2(exp1);           /* 2^(2k+1) */
    seq_t p2 = pow2(exp2);           /* 2^(2k - m) */

    return p1 - p2;                  /* 2^(2k+1) - 2^(2k - m) */
}
