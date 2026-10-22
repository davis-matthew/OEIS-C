/*
 * A321516  Number of composite divisors of n that are < n.
 * https://oeis.org/A321516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(0, tau(n) - omega(n) - 2) * tau(n) – number of
 *            divisors of n * omega(n) – number of distinct prime factors of n
 *            * The function works for any n ≥ 1 using simple trial division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    uint64_t m = n;
    uint64_t tau = 1;          /* total divisor count */
    uint64_t distinct = 0;     /* distinct prime factors */

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        distinct++;
        uint64_t exp = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            exp++;
        }
        tau *= (exp + 1);
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            distinct++;
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1) {
        distinct++;
        tau *= 2;
    }

    /* a(n) = tau - distinct - 2, but never negative */
    if (tau <= distinct + 2)
        return (seq_t)0;

    return (seq_t)(tau - distinct - 2);
}
