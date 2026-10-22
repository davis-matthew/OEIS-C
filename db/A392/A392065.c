/*
 * A392065  a(n) = number of primes with unit exponent in the prime
 *          factorization of n, or 1 if no such primes exist.
 * https://oeis.org/A392065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(A056169(n), 1) (number of primes with exponent 1 in
 *            n, or 1 if none)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t val = (uint64_t)n;
    if (val <= 1)
        return 1;

    uint64_t remaining = val;
    uint64_t count = 0;

    for (uint64_t p = 2; p * p <= remaining; ++p)
    {
        if (remaining % p == 0)
        {
            uint64_t exp = 0;
            while (remaining % p == 0)
            {
                remaining /= p;
                ++exp;
            }
            if (exp == 1)
                ++count;
        }
    }

    if (remaining > 1)          /* last prime factor (exponent 1) */
        ++count;

    return count > 0 ? (seq_t)count : (seq_t)1;
}
