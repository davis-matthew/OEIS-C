/*
 * A273418  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 705", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=4, for n>1 a(n)=4*4^n - 4*2^n - 7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small, pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t term = pow4(n + 1);                     /* 4^(n+1) */
    seq_t sub  = mul_small(pow2(n), (unsigned)4); /* 4 * 2^n   */
    term = term - sub;
    term = term - (seq_t)7;
    return term;
}
