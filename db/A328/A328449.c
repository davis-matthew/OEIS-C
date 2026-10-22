/*
 * A328449  Smallest number in whose divisors the longest run is of length n,
 *          and 0 if none exists.
 * https://oeis.org/A328449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM(1,2,…,n) if n+1 is a prime power, otherwise 0.
 *            Offset: 0 (so a(0)=0)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 23, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   build_primes_upto, largest_prime_divisor, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"          /* seq_mul */
#include "../utils/build_primes_upto.h"/* build_primes_upto */
#include "../utils/largest_prime_divisor.h"/* largest_prime_divisor */

static idx_t *prime_cache = NULL;    /* 0‑based array of primes */
static size_t prime_cnt  = 0;         /* number of primes cached */
static idx_t  prime_limit = 0;       /* largest n for which primes are cached */

/* Ensure primes up to `limit` are available in `prime_cache`;
   free old cache if a larger limit is requested. */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_cache && prime_limit >= limit)
        return;                /* already sufficient */

    /* free old cache */
    if (prime_cache)
        free(prime_cache);

    build_primes_upto(limit, (idx_t **)&prime_cache, &prime_cnt);
    prime_limit = limit;
}

/* Return true iff m is a prime power (m = p^k with p prime, k ≥ 1) */
static bool is_prime_power(idx_t m)
{
    if (m <= 1)
        return false;          /* 1 is not a prime power for this sequence */

    uint64_t p = largest_prime_divisor(m);
    if (p == 0)                /* safety guard: should not happen for m>1 */
        return false;

    while (m % p == 0)
        m /= p;

    return m == 1;
}

/* Compute LCM(1,2,…,n) as a product of prime powers */
static seq_t lcm_upto(idx_t n)
{
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt && prime_cache[i] <= n; ++i)
    {
        seq_t p = (seq_t)prime_cache[i];
        seq_t power = p;           /* start with p^1 */
        seq_t next = seq_mul(power, p); /* p^2 */

        /* grow power while it stays ≤ n */
        while (next <= (seq_t)n) {
            power = next;
            next = seq_mul(power, p);
        }

        result = seq_mul(result, power);  /* multiply by this prime power */
    }

    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 */

    idx_t m = n + 1;              /* check if n+1 is a prime power */

    /* Ensure we have sufficient primes for both the test and LCM */
    ensure_primes_upto(m);

    if (!is_prime_power(m))
        return (seq_t)0;          /* no suitable divisor run */

    /* n+1 is a prime power – return LCM(1…n) */
    return lcm_upto(n);
}
