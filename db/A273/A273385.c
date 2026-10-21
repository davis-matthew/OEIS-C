/*
 * A273385  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 659", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1, a(1) = 5; a(n) = 4*4^n - 4*2^n + 1 for n >= 2 (OEIS
 *            A273385)
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
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    /* 4*4^n = 4 * pow4(n) */
    seq_t term = (seq_t)4 * pow4(n);
    /* subtract 4*2^n */
    term -= (seq_t)4 * pow2(n);
    /* add constant 1 */
    term += (seq_t)1;
    return term;
}
