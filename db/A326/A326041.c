/*
 * A326041  a(n) = sigma(A064989(n)).
 * https://oeis.org/A326041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(A064989(n)) a is multiplicative: a(2^e)=1, and for
 *            odd primes p a(p^e) = (prev_prime(p)^(e+1)-1)/(prev_prime(p)-1)
 * Range:     u8 n <= 18, u16 n <= 1657, u32 n <= 9829, u64 n <= 9829,
 *            u128 n <= 9829, big any n
 * Helpers:   pow_uint, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prev_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* safety guard */

    uint64_t m = (uint64_t)n;                  /* work on uint64_t */
    seq_t   result = (seq_t)1;

    /* remove all factors of 2 – they contribute 1 */
    while ((m & 1ULL) == 0ULL) m >>= 1;

    /* trial division for odd primes */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0ULL) {
            int e = 0;
            while (m % p == 0ULL) {
                ++e;
                m /= p;
            }
            /* p is odd prime, compute its contribution */
            idx_t q = prev_prime(p);
            seq_t power     = pow_uint((uint64_t)q, (idx_t)(e + 1));
            seq_t numerator = power - (seq_t)1;
            seq_t denom     = (seq_t)(q - 1);
            seq_t term      = numerator / denom;
            result         *= term;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1ULL) {
        uint64_t p = m;                         /* exponent = 1 */
        idx_t q = prev_prime(p);
        seq_t power     = pow_uint((uint64_t)q, (idx_t)2);   /* e+1 = 2 */
        seq_t numerator = power - (seq_t)1;
        seq_t denom     = (seq_t)(q - 1);
        seq_t term      = numerator / denom;
        result          *= term;
    }

    return result;
}
