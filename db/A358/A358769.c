/*
 * A358769  a(n) = 1 if n is of the form p * m^2, where p is a prime and m is
 *          a natural number >= 1, otherwise 0.
 * https://oeis.org/A358769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is of the form p * m^2 (p prime, m≥1), otherwise 0
 * Range:     u8 n <= 9861, u16 n <= 9861, u32 n <= 9861, u64 n <= 9861,
 *            u128 n <= 9861, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to working integer type */
    uint64_t x = (uint64_t)n;

    /* n < 2 cannot be of the required form */
    if (x < 2)
        return (seq_t)0;

    int odd = 0;                    /* count of prime exponents that are odd */

    /* Handle prime factor 2 separately (even factor) */
    if ((x & 1ULL) == 0U) {
        int e = 0;
        while ((x & 1ULL) == 0U) {
            x >>= 1ULL;
            ++e;
        }
        if ((e & 1) != 0)
            ++odd;
        if (odd > 1)
            return (seq_t)0;
    }

    /* Check remaining odd prime factors */
    for (uint64_t p = 3ULL; p <= x / p; p += 2ULL) {
        if (x % p == 0ULL) {
            int e = 0;
            while (x % p == 0ULL) {
                x /= p;
                ++e;
            }
            if ((e & 1) != 0)
                ++odd;
            if (odd > 1)
                return (seq_t)0;
        }
    }

    /* If any prime factor remains, it appears exactly once (exponent 1) */
    if (x > 1ULL) {
        ++odd;
        if (odd > 1)
            return (seq_t)0;
    }

    /* Return 1 only if exactly one prime exponent is odd */
    return (seq_t)(odd == 1);
}
