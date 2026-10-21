/*
 * A273327  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 646", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 19*4^(n-2) - 16*3^(n-3) + 1 for n>2, with a(0)=1, a(1)=5,
 *            a(2)=17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 17;

    seq_t pow4 = pow_uint(4, n - 2);
    seq_t pow3 = pow_uint(3, n - 3);

    return (seq_t)19 * pow4 - (seq_t)16 * pow3 + 1;
}
