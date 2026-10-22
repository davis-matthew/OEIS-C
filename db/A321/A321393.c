/*
 * A321393  a(n) is the number of bases b > 1 such that n + digitsum(n, base
 *          b) is prime.
 * https://oeis.org/A321393
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of bases b > 1 such that n + digitsum(n, base
 *            b) is prime
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* offset is 2 */

    seq_t count = 0;

    for (idx_t b = 2; b <= n; ++b)
    {
        seq_t sum   = 0;
        seq_t temp  = (seq_t)n;
        seq_t base  = (seq_t)b;

        while (temp > 0)
        {
            sum += temp % base;
            temp /= base;
        }

        seq_t candidate = (seq_t)n + sum;
        if (is_prime_small(candidate))
            ++count;
    }

    return count;
}
