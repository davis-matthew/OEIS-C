/*
 * A337273  Number of distinct positive integer pairs, (s,t), such that s < t
 *          < n where neither s nor t divides n.
 * https://oeis.org/A337273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n - tau(n), 2) where tau(n) is the number of
 *            divisors of n
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Compute tau(n) = divisor_count_u64(n) */
    seq_t tau = divisor_count_u64(n);

    /* m = n - tau(n) */
    seq_t m = (seq_t)n - tau;

    /* If there are less than two eligible numbers, result is zero */
    if (m < 2)
        return 0;

    /* Compute binomial(m, 2) = m * (m-1) / 2 */
    return (m * (m - 1)) / 2;
}
