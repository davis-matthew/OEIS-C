/*
 * A345303  a(n) = Sum_{p|n, p prime} p * gcd(p,n/p).
 * https://oeis.org/A345303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p * gcd(p, n/p)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(1) = 0, all other terms defined for n >= 1 */
    if (n < 1)
        return (seq_t)0;

    uint64_t orig = (uint64_t)n;          /* keep original n for n/p calculations */
    uint64_t tmp  = orig;                 /* temporary variable for factorization */
    seq_t result  = 0;

    /* First handle factor 2 explicitly to avoid redundant checks */
    if (tmp % 2 == 0) {
        seq_t p = 2;
        result += p * gcd(p, (seq_t)(orig / 2));
        while (tmp % 2 == 0)
            tmp /= 2;
    }

    /* Iterate odd candidates up to sqrt(tmp) */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            seq_t prime = (seq_t)p;
            result += prime * gcd(prime, (seq_t)(orig / p));
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* If remaining tmp is a prime > sqrt(orig) */
    if (tmp > 1) {
        seq_t prime = (seq_t)tmp;
        result += prime * gcd(prime, (seq_t)(orig / tmp));
    }

    return result;
}
