/*
 * A270682  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 190", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4^n + 4*2^n - 8 for n > 1; a(0) = 1, a(1) = 5
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
    if (n == 1)
        return (seq_t)5;

    seq_t pow2_n = pow2(n);          /* 2^n        */
    seq_t pow4_n = pow2_n * pow2_n;  /* 4^n = (2^n)^2 */

    return pow4_n + (seq_t)4 * pow2_n - (seq_t)8;
}
