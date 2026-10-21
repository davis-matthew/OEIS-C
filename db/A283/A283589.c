/*
 * A283589  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 657", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=101, a(n)=10*(10^n-1)/9 for n>2 (10^n-1 is
 *            divisible by 9, so no loss of precision)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)101;

    seq_t pow10_n = pow10(n);          /* 10^n in seq_t */
    seq_t part = (pow10_n - (seq_t)1) / (seq_t)9; /* (10^n-1)/9 */
    return part * (seq_t)10;                 /* ×10  */
}
