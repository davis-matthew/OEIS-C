/*
 * A390299  Number of positive z <= n such that z^2 - 9^g is relatively prime
 *          to n (for any particular positive integer g).
 * https://oeis.org/A390299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} f(p,e) where f(2,e) = 2^(e-1), f(3,e) =
 *            2·3^(e-1), f(p,e) = (p-2)·p^(e-1) for p ≥ 5
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------
   Utility: integer square‑root of an uint64_t (idx_t).  Works for all
   bit widths of idx_t that are at most 64 bits.  No floating point
   operations are used.
   ------------------------------------------------------------------ */
static idx_t isqrt_idx(idx_t x)
{
    idx_t res = 0;
    /* Highest power of 4 ≤ x */
    idx_t bit = ((idx_t)1) << (((sizeof(idx_t) * 8) - 2) & ~1);
    while (bit > 0)
    {
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* ------------------------------------------------------------------
   Cached prime sieve up to a required limit.  The array is rebuilt
   only when the new limit exceeds the current one.
   ------------------------------------------------------------------ */
#include "../utils/build_primes_upto.h"

static uint64_t *prime_cache = NULL;
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;

static void ensure_prime_cache(idx_t limit)
{
    if (prime_cache != NULL && prime_limit >= limit)
        return;

    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* Free previous cache if it exists */
    if (prime_cache != NULL)
        free(prime_cache);

    prime_cache = new_primes;
    prime_cnt   = new_cnt;
    prime_limit = limit;
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but guard */
    /* a(1) = 1 by definition */
    if (n == 1)
        return (seq_t)1;

    /* Factor n using primes up to sqrt(n) */
    idx_t limit = isqrt_idx(n);
    if (limit < 2)
        limit = 2;
    ensure_prime_cache(limit);

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_cache[i];
        if (p * p > m)
            break;
        if (m % p != 0)
            continue;

        uint64_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        /* compute p^(e-1) as seq_t */
        seq_t pow = (seq_t)1;
        for (uint64_t k = 1; k < e; ++k)
            pow *= (seq_t)p;

        seq_t contrib;
        if (p == 2ull)
        {
            contrib = pow;                      /* 2^(e-1) */
        }
        else if (p == 3ull)
        {
            contrib = pow * (seq_t)2;            /* 2·3^(e-1) */
        }
        else
        {
            contrib = pow * (seq_t)(p - 2ull);   /* (p-2)·p^(e-1) */
        }
        result *= contrib;
    }

    /* Remaining prime factor (if any) */
    if (m > 1)
    {
        uint64_t p = m;          /* exponent = 1, so pow = 1 */
        seq_t contrib;
        if (p == 2ull)
        {
            contrib = (seq_t)1;                 /* 2^(1-1) */
        }
        else if (p == 3ull)
        {
            contrib = (seq_t)2;                 /* 2·3^(1-1) */
        }
        else
        {
            contrib = (seq_t)(p - 2ull);        /* (p-2)·p^(1-1) */
        }
        result *= contrib;
    }

    return result;
}
