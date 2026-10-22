/*
 * A392321  Number of all 1-tuples, 2-tuples, ..., n-tuples where each entry
 *          is chosen from the nonempty subsets of {1,..,n}.
 * https://oeis.org/A392321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (2^n - 1)^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* b = 2^n - 1 */
    seq_t b = pow2(n);
    b -= (seq_t)1;

    seq_t sum  = (seq_t)0;
    seq_t term = b;          /* term = b^1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        sum += term;
        if (i == n)
            break;
        term *= b;           /* prepare next power */
    }

    return sum;
}
