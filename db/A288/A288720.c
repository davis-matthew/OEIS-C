/*
 * A288720  Detour index of the n-hypercube graph.
 * https://oeis.org/A288720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, n = 0 * a(n) = 2^(3n‑1) + 2^n – 5·2^(2n‑2), n > 0 *
 *            (equivalent to 2ⁿ·(2^(2n‑1) – 5·2^(n‑2) + 1) for integer n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* term1 = 2^(3n-1) */
    seq_t term1 = pow2((idx_t)(3 * n - 1));
    /* term2 = 2^n */
    seq_t term2 = pow2(n);
    /* term3 = 5 * 2^(2n-2) */
    seq_t term3 = (seq_t)5 * pow2((idx_t)(2 * n - 2));

    return term1 + term2 - term3;
}
