/*
 * A366077  The number of prime factors of the cubefree part of n (A360539),
 *          counted with multiplicity.
 * https://oeis.org/A366077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the exponents < 3 in the prime factorization of n
 *            (additive with a(p^e) = e if e ≤ 2, and a(p^e) = 0 for e ≥ 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 1. */
    if (n <= 1)
        return 0;

    uint64_t t = (uint64_t)n;
    seq_t result = 0;

    while (t > 1) {
        uint64_t p = _smallest_prime_factor(t);
        uint64_t e = 0;

        /* Count the exponent of p in t. */
        while (t % p == 0) {
            t /= p;
            ++e;
        }

        /* Add the exponent only when it is 1 or 2. */
        if (e <= 2)
            result += (seq_t)e;
    }

    return result;
}
