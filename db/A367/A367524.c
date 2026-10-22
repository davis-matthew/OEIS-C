/*
 * A367524  The number of ways of tiling the n X n grid up to the symmetries
 *          of the square by a tile that is fixed under horizontal reflection,
 *          but no other symmetries of the square.
 * https://oeis.org/A367524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(4m^2-4m-2) * (2 + 2^((2m-1)^2)) if n = 2m-1 (odd n)
 *            a(n) = 2^(2m^2-3) * (2 + 3*4^(m^2) + 64^(m^2)) if n = 2m (even
 *            n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        /* offset is 1; behavior for n<1 is undefined */
        return (seq_t)0;
    }
    if (n & 1u) { /* odd n */
        if (n == 1u) return (seq_t)1;          /* handles the -2 exponent case */
        idx_t m = (n + 1u) / 2u;               /* m >= 2 for n >= 3 */
        idx_t exp_outer = (idx_t)4 * m * m - (idx_t)4 * m - (idx_t)2;
        seq_t outer = pow2(exp_outer);
        idx_t exp_inner = n * n;               /* (2m-1)^2 = n^2 */
        seq_t inner_half = pow2(exp_inner);
        seq_t inner = (seq_t)2 + inner_half;
        return outer * inner;
    } else { /* even n */
        if (n == 2u) return (seq_t)39;         /* handles the -1 exponent case */
        idx_t m = n / 2u;                      /* m >= 2 for n >= 4 */
        idx_t exp_outer = (idx_t)2 * m * m - (idx_t)3;
        seq_t outer = pow2(exp_outer);
        /* 4^(m^2) = 2^(2*m^2) */
        idx_t exp_4 = (idx_t)2 * m * m;
        seq_t term_4 = pow2(exp_4);
        /* 64^(m^2) = 2^(6*m^2) */
        idx_t exp_64 = (idx_t)6 * m * m;
        seq_t term_64 = pow2(exp_64);
        seq_t inner = (seq_t)2 + (seq_t)3 * term_4 + term_64;
        return outer * inner;
    }
}
