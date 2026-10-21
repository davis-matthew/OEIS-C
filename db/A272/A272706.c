/*
 * A272706  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 515", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*4^n - 8*2^n + 9 for n>=1; a(0)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t p2 = pow2(n);            /* 2^n */
    seq_t p4 = seq_mul(p2, p2);    /* (2^n)^2 = 4^n */

    seq_t res = 3 * p4 - 8 * p2 + 9;
    return res;
}
