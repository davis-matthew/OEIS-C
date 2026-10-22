/*
 * A385392  The number of divisors d of n such that -(d^d) == d (mod n).
 * https://oeis.org/A385392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{d | n : d^d + d ≡ 0 (mod n)}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            seq_t val = (seq_t)d;
            if ((mod_pow(val, (uint64_t)d, (seq_t)n) + val) % (seq_t)n == 0)
                ++count;

            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t val2 = (seq_t)d2;
                if ((mod_pow(val2, (uint64_t)d2, (seq_t)n) + val2) % (seq_t)n == 0)
                    ++count;
            }
        }
    }
    return count;
}
