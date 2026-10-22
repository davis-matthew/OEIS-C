/*
 * A359579  Dirichlet inverse of A336923, where A336923(n) = 1 if sigma(2n) -
 *          sigma(n) is a power of 2, otherwise 0.
 * https://oeis.org/A359579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1. For n > 1, write n = 2^e0 * Π p_i^e_i with p_i odd
 *            primes. If e0 > 1 → a(n) = 0. Otherwise let s = –1 if e0 = 1
 *            else 1. For each odd prime factor p_i: if p_i + 1 is not a power
 *            of two → a(n) = 0; else if e_i is odd → s = –s. a(n) = s.
 *            (Equivalently, for each odd prime p, a(p^e) = (–1)^e...
 * Range:     i8 n <= 126, i16 n <= 9850, i32 n <= 9850, i64 n <= 9850,
 *            i128 n <= 9850, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    /* Handle factor 2 separately */
    if ((temp & 1ULL) == 0) {
        unsigned e2 = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e2;
            if (e2 > 1) return 0;   /* 2^e with e > 1 gives a = 0 */
        }
        if (e2 == 1) result = -1;    /* contribution of a(2) */
    }

    /* Factor the remaining odd part */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* p is prime */
            if (!is_power_of_two((seq_t)p + 1)) return 0;
            if (exp & 1) result = -result;   /* flip sign for odd exponent */
        }
    }

    /* Handle a final prime factor > sqrt(n), if any */
    if (temp > 1) {
        /* temp is prime */
        if (!is_power_of_two((seq_t)temp + 1)) return 0;
        result = -result;   /* exponent is 1 */
    }

    return result;
}
