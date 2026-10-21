/*
 * A269712  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 20", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1; a(n)=4*(2^n-1) for n>0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Fast closed‑form using 2^n */
    if (n == 0)
        return (seq_t)1;

    seq_t two_n = pow2(n);      /* 2^n */
    seq_t res = two_n - 1;      /* (2^n)-1  */
    res *= 4;                   /* 4*((2^n)-1) */
    return res;
}
