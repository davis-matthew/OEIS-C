/*
 * A271017  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 251", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1 a(n) = 3 * 4^n - 7 for n > 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= (seq_t)4;

    return (seq_t)3 * pow4 - (seq_t)7;
}
