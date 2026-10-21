/*
 * A273767  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 913", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(0)=1, a(1)=4, a(2)=48; for n>=3
   a(n) = (2^(n+1)-1)^2  (conjectured formula) */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)48;

    seq_t val = pow2(n + 1);     /* 2^(n+1) */
    seq_t t   = val - (seq_t)1;  /* 2^(n+1) - 1 */
    return t * t;                /* (2^(n+1)-1)^2 */
}
