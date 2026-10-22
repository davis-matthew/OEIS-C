/*
 * A368542  The number of divisors of n whose prime factors are all Mersenne
 *          primes (A000668).
 * https://oeis.org/A368542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p is a Mersenne prime ? e+1 : 1) (OEIS
 *            A368542)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Known Mersenne primes that fit in 64 bits */
static const uint64_t mersenne_primes[] = {
    3ULL,          /* 2^2-1 */
    7ULL,          /* 2^3-1 */
    31ULL,         /* 2^5-1 */
    127ULL,        /* 2^7-1 */
    8191ULL,       /* 2^13-1 */
    131071ULL,     /* 2^17-1 */
    524287ULL,     /* 2^19-1 */
    2147483647ULL, /* 2^31-1 */
    2305843009213693951ULL /* 2^61-1 */
};
static const size_t mersenne_count = sizeof(mersenne_primes) / sizeof(mersenne_primes[0]);

/* Small helper to test if a prime is a Mersenne prime */
static inline int is_mersenne_prime(uint64_t p)
{
    for (size_t i = 0; i < mersenne_count; ++i)
        if (p == mersenne_primes[i])
            return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for 0, return 0 for safety */
        return 0;
    uint64_t N = (uint64_t)n;

    /* Enough space for all distinct prime factors of a 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  count = 0;

    factor_u64(N, primes, exps, &count);

    seq_t result = 1;
    for (size_t i = 0; i < count; ++i)
    {
        if (is_mersenne_prime(primes[i]))
        {
            result *= (seq_t)(exps[i] + 1);
        }
    }
    return result;
}
