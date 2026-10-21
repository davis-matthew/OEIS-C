/*
 * A273676  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 833", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1 a(1) = 4 for n > 1: a(n) = 3*4^n - 4*2^n + 1
 *            (equivalently a(n) = 3*2^(2n) - 4*2^n + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t pow4  = pow2(2 * n);           /* 2^(2n) = 4^n */
    seq_t term1 = 3 * pow4;              /* 3*4^n */
    seq_t pow2n = pow2(n);               /* 2^n */
    seq_t term2 = 4 * pow2n;             /* 4*2^n */

    return term1 - term2 + (seq_t)1;
}
