/*
 * A322582  a(n) = n - A003958(n), where A003958 is fully multiplicative with
 *          a(p) = (p-1).
 * https://oeis.org/A322582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - product_{p^e∥n} (p-1)^e (fully multiplicative
 *            A003958)
 * Range:     u8 n <= 271, u16 n <= 9866, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* factor n into primes, computing the product (p-1)^e on the fly   */
    uint64_t nn = n;          /* mutable copy of n for factorization */
    seq_t prod = 1;           /* running product of (p-1)^e */

    /* Handle factor 2 separately (p-1 = 1, so it does not change prod) */
    uint64_t e = 0;
    while (nn % 2 == 0) {
        ++e;
        nn /= 2;
    }
    /* (2-1)^e == 1, so nothing to multiply */

    /* Trial division by odd primes up to sqrt(nn) */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            e = 0;
            while (nn % p == 0) {
                ++e;
                nn /= p;
            }
            seq_t base = (seq_t)(p - 1);
            for (uint64_t i = 0; i < e; ++i) {
                prod *= base;
            }
        }
    }

    /* If remainder nn > 1, it's a prime factor > sqrt(original n) */
    if (nn > 1) {
        seq_t base = (seq_t)(nn - 1);
        prod *= base;
    }

    /* a(n) = n - product */
    return (seq_t)n - prod;
}
