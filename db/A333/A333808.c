/*
 * A333808  Sum of distinct prime divisors of n that are < sqrt(n).
 * https://oeis.org/A333808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sum of distinct prime divisors of n that are < sqrt(n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   my_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/my_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    uint64_t orig = (uint64_t)n;
    uint64_t limit = my_isqrt_u64(orig);
    uint64_t rem   = orig;
    seq_t sum = 0;

    for (uint64_t p = 2; p <= limit; ++p)
    {
        if (rem % p == 0)
        {
            if (((unsigned __int128)p * p) < orig)
                sum += (seq_t)p;
            while (rem % p == 0)
                rem /= p;
        }
    }

    if (rem > 1)
    {
        if (((unsigned __int128)rem * rem) < orig)
            sum += (seq_t)rem;
    }

    return sum;
}
