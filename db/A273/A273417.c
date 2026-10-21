/*
 * A273417  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 705", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-15-(-1)^n+8*n+8*n^2)/2 for n > 2, with a(0)=1, a(1)=4,
 *            a(2)=21
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)21;

    int64_t nn   = (int64_t)n;
    int64_t pow  = (n & 1) ? (int64_t)-1 : (int64_t)1;   /* (-1)^n */
    int64_t num  = (int64_t)8 * nn * nn + (int64_t)8 * nn
                   - (int64_t)15 - pow;
    return (seq_t)(num / (int64_t)2);
}
