/*
 * A350105  Number of subsets of the initial segment of the natural numbers
 *          strictly below n which are not self-measuring. Number of subsets S
 *          of [n] with S != distset(S).
 * https://oeis.org/A350105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - ( 2 + Σ_{d=1}^{n-1} floor((n-1)/d) )
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n >= 0, with a(0) = 0 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = n - 1;
    seq_t sum = 2;                /* counts the empty set and {0}   */
    for (idx_t d = 1; d <= m; ++d)
        sum += (seq_t)(m / d);    /* add all sets {0, d, 2d, …, k*d} with k >= 1 */

    seq_t pow = pow2(n);          /* 2^n in seq_t */
    return pow - sum;
}
