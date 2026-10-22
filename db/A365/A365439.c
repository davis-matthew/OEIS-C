/*
 * A365439  a(n) = Sum_{k=1..n} binomial(floor(n/k)+4,5).
 * https://oeis.org/A365439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(floor(n/k)+4,5) (OEIS A365439)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 (offset 1). */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m = n / k;              /* floor(n/k) */
        sum += binom(m + 4, 5);       /* binomial(m+4,5) */
    }
    return sum;
}
