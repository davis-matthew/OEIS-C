/*
 * A363913  a(n) = Sum_{k=0..n} divides(k, n) * 3^k, where divides(k, n) = 1
 *          if k divides n, otherwise 0.
 * https://oeis.org/A363913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} [k divides n] * 3^k, where [condition] is 1
 *            if k divides n, otherwise 0. For n = 0 the only term is 3^0 = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 1;                 /* 3^0 = 1 */

    seq_t sum = 0;

    /* Enumerate all divisors of n by iterating up to sqrt(n). */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            sum += pow_seq((seq_t)3, d);

            /* paired divisor n/d (if distinct) */
            idx_t other = n / d;
            if (other != d)
                sum += pow_seq((seq_t)3, other);
        }
    }

    return sum;
}
