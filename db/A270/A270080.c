/*
 * A270080  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 62", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=5, a(2)=20; for n>2, a(n) = 4^n + 2*2^n - 8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)20;
    seq_t pow2_n   = pow2(n);        /* 2^n */
    seq_t pow2_2n  = pow2(2 * n);    /* 4^n = 2^(2n) */
    seq_t result = pow2_2n + (seq_t)2 * pow2_n;
    result -= (seq_t)8;
    return result;
}
