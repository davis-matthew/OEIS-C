/*
 * A270126  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 86", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=5, a(n)=3*2^(2*n-1) for n>=2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    idx_t exp = (n << 1) - 1;   /* 2*n - 1 */
    seq_t val = pow2(exp);      /* 2^(2*n-1) */
    return val * (seq_t)3;      /* 3 * 2^(2*n-1) */
}
