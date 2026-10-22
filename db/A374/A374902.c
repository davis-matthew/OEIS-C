/*
 * A374902  Numerator of the mean number of divisors of the divisors of n.
 * https://oeis.org/A374902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of (sum_{d|n} tau(d))/tau(n) after reduction
 * Range:     u8 n <= 9239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from multiplicativity: f(p^e) = (e+2)/2, so f(n) = Π(e_i+2) / 2^k */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    uint64_t m = (uint64_t)n;          /* work on a 64-bit copy for factorisation */
    seq_t prod = 1;                     /* product Π(e_i+2) */
    idx_t distinct = 0;                 /* number of distinct prime factors k */

    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            do {                        /* count exponent e of prime p */
                t /= p;
                ++e;
            } while (t % p == 0);
            prod *= (seq_t)(e + 2);
            ++distinct;
        }
    }
    if (t > 1) {                         /* remainder is a prime factor */
        prod *= (seq_t)(1 + 2);          /* e = 1 -> factor 3 */
        ++distinct;
    }

    /* cancel common powers of 2 between numerator and denominator */
    idx_t v2 = 0;
    seq_t tmp = prod;
    while ((tmp & 1) == 0) {             /* count how many 2's divide prod */
        tmp >>= 1;
        ++v2;
    }
    idx_t cancel = (v2 < distinct) ? v2 : distinct;
    prod >>= cancel;                     /* remove 2^cancel from numerator */

    return prod;
}
