/*
 * A321579  Number of n-tuples of 4 elements excluding reverse duplicates and
 *          those consisting of repetitions of the same element only.
 * https://oeis.org/A321579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n-2)*((-1)^(n+1) + 3) + 2^(2*n-1) - 4) for n > 0 a(0)
 *            = 0, a(1) = 0 Computes A321579: Number of n‑tuples of 4 elements
 *            excluding reverse duplicates and those consisting of repetitions
 *            of the same element only
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* Provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n) {
    if (n < 2)
        return 0; /* a(0) = a(1) = 0 */

    /* 2^(n-2) and 2^(2*n-1) */
    seq_t pow_n2   = pow2(n - 2);
    seq_t pow_2n1  = pow2((idx_t)(2 * n - 1));

    /* Compute (-1)^(n+1) + 3, which is 2 for even n and 4 for odd n */
    int sign = (n & 1) ? 1 : -1;              /* n odd -> sign=1, n even -> sign=-1 */
    int factor = sign + 3;                    /* factor = 4 (odd) or 2 (even) */

    seq_t term1 = pow_n2 * (seq_t)factor;     /* 2^(n-2) * factor */
    seq_t term2 = pow_2n1 - 4;                /* 2^(2n-1) - 4 */

    return term1 + term2;                     /* a(n) */
}
