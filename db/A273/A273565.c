/*
 * A273565  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 793", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 43*4^(n-2) – 4*2^n + 1 for n>1, a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;

    seq_t term1 = pow4(n - 2);        /* 4^(n-2) */
    term1 = term1 * (seq_t)43;        /* 43*4^(n-2) */

    seq_t term2 = pow2(n);            /* 2^n */
    term2 = term2 * (seq_t)4;         /* 4*2^n */

    seq_t result = term1 - term2 + (seq_t)1;
    return result;
}
