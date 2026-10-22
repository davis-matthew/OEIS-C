/*
 * A331045  a(n) is the least prime number of the form floor(n/10^k) for some
 *          k >= 0, or 0 if no such prime number exists.
 * https://oeis.org/A331045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the least prime among the decimal prefixes of n, or 0 if
 *            no such prime exists
 * Range:     u8 n <= 400, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t best = 0;
    while (cur > 0)                /* iterate over all decimal prefixes */
    {
        if (is_prime_big(cur))     /* record the prime – it is the smallest
                                      because prefixes decrease afterwards */
            best = cur;
        cur /= 10;                 /* drop the last digit */
    }
    return best;                   /* 0 if no prime prefix found */
}
