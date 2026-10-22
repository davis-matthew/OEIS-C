/*
 * A339903  Fully multiplicative with a(p) = A000265(q-1), where q =
 *          A151800(p), the next prime > p.
 * https://oeis.org/A339903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is fully multiplicative with a(p) = largest odd divisor of
 *            (next_prime(p)-1)
 * Range:     u8 n <= 520, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"

static seq_t a_prime_value(uint64_t p)
{
    /* p is prime.  Compute next prime q > p, then return the largest odd divisor of q-1. */
    uint64_t q = next_prime(p);          /* next_prime returns the smallest prime > p  */
    uint64_t t = q - 1;                   /* q-1 is even for any prime q > 2               */
    while ((t & 1U) == 0U) t >>= 1U;      /* strip off powers of 2 to get odd part      */
    return (seq_t)t;                      /* cast to seq_t for the result                 */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1ULL;       /* safety; OEIS offset starts at 1.              */

    /* Factor n. Use at most 64 primes (enough for n <= 2^64).               */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        seq_t ap = a_prime_value(primes[i]);      /* a(p) for the prime factor          */
        for (uint64_t e = 0; e < exps[i]; ++e) {
            result *= ap;                        /* multiplicative: a(p^e) = a(p)^e     */
        }
    }
    return result;
}
