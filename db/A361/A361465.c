/*
 * A361465  a(n) = 1 if A017665(n) [the numerator of the sum of the
 *          reciprocals of the divisors of n] is a power of 2, otherwise 0.
 * https://oeis.org/A361465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the numerator of sigma(n)/n in lowest terms is a
 *            power of 2, otherwise 0. sigma(n) = sum of positive divisors of
 *            n
 * Range:     u8 n <= 2188, u16 n <= 6991, u32 n <= 6991, u64 n <= 6991,
 *            u128 n <= 6991, big any n
 * Helpers:   gcd, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_power_of_two.h"

static seq_t sigma_of_uint64(uint64_t n)
{
    seq_t sum = 0;
    for (uint64_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            sum += (seq_t)d;
            uint64_t e = n / d;
            if (e != d)
                sum += (seq_t)e;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    seq_t s = sigma_of_uint64(n64);
    seq_t g = gcd(s, (seq_t)n64);
    seq_t num = s / g;   /* numerator of sigma(n)/n in lowest terms */
    return is_power_of_two(num) ? (seq_t)1 : (seq_t)0;
}
