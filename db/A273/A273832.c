/*
 * A273832  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 961", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 4*2^n - 3 for n > 1 Init: a(0)=1, a(1)=4. The
 *            recurrence a(n)=7*a(n-1)-14*a(n-2)+8*a(n-3) also holds for n>4
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
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t term1 = pow4(n) * (seq_t)4;   /* 4*4^n */
    seq_t term2 = pow2(n) * (seq_t)4;   /* 4*2^n */

    seq_t result = term1;
    result -= term2;
    result -= (seq_t)3;
    return result;
}
