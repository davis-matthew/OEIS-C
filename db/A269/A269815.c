/*
 * A269815  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 35", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1; a(n) = 14*4^(n-1) - 5*2^n + 1 for n>0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* 4^(n-1) = 2^(2*(n-1)) */
    idx_t exp4 = (n - 1) << 1;            /* 2*(n-1) */
    seq_t term = pow2(exp4);              /* 4^(n-1) */
    term = term * 14;                     /* 14*4^(n-1) */

    seq_t power2 = pow2(n);               /* 2^n */
    power2 = power2 * 5;                  /* 5*2^n */

    term = term - power2;                 /* 14*4^(n-1) - 5*2^n */
    term = term + 1;                      /* +1 */

    return term;
}
