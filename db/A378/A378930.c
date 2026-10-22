/*
 * A378930  a(n) = n * d(n) / gcd(n, d(n))^2, where d = A000005.
 * https://oeis.org/A378930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * d(n) / gcd(n, d(n))^2 where d(n) is the number of
 *            divisors of n (A000005)
 * Range:     u8 n <= 35, u16 n <= 2099, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Treat n == 0 as undefined; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Compute d(n).  divisor_count_u64 expects uint64_t, which our idx_t is. */
    seq_t d = divisor_count_u64((uint64_t)n);

    /* Compute g = gcd(n, d). */
    seq_t g = gcd((seq_t)n, d);

    /* Compute n * d / g^2.  Use intermediate variable for g^2. */
    seq_t g2 = g * g;
    seq_t prod = (seq_t)n * d;
    return prod / g2;
}
