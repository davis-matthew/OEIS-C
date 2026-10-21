/*
 * A270935  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 221", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 4*2^n - 4 (for n ≥ 2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint_impl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint_impl.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 8;
    /* Compute 4^n and 2^n */
    seq_t pow4 = pow_uint_impl(4, n);
    seq_t pow2 = pow_uint_impl(2, n);
    /* a(n) = 4*4^n - 4*2^n - 4 */
    seq_t res = pow4;
    res *= 4;                  /* 4*4^n */
    seq_t sub = pow2;
    sub *= 4;                  /* 4*2^n */
    res -= sub;
    res -= 4;
    return res;
}
