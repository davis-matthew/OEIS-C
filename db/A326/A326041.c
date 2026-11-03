/*
 * A326041  a(n) = sigma(A064989(n)).
 * https://oeis.org/A326041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(A064989(n)) a is multiplicative: a(2^e)=1, and for
 *            odd primes p a(p^e) = (prev_prime(p)^(e+1)-1)/(prev_prime(p)-1)
 * Range:     u8 n <= 18, u16 n <= 1657, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 104 terms through n = 9829 (2026-11-01)
 * Helpers:   ipow_u64_idx, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/is_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

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
            seq_t power     = ipow_u64_idx((uint64_t)q, (idx_t)(e + 1));
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
        seq_t power     = ipow_u64_idx((uint64_t)q, (idx_t)2);   /* e+1 = 2 */
        seq_t numerator = power - (seq_t)1;
        seq_t denom     = (seq_t)(q - 1);
        seq_t term      = numerator / denom;
        result          *= term;
    }

    return result;
}
