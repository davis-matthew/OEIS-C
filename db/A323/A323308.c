/*
 * A323308  The number of exponential semiproper divisors of n.
 * https://oeis.org/A323308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^k where k = #{p | p^2 divides n} (number of primes that
 *            appear in n with exponent at least 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t temp = (uint64_t)n;
    idx_t   count = 0;

    /* factor 2 */
    idx_t exp = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        exp++;
    }
    if (exp >= 2) count++;

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (exp >= 2) count++;
        }
    }

    /* remaining prime factor (exponent 1) has no effect on count */
    /* (temp > 1) */

    seq_t res = 1;
    for (idx_t i = 0; i < count; ++i)
        res = res * 2;

    return res;
}
