/*
 * A246911  Numbers n such that sigma(n+sigma(n)) = 4*sigma(n).
 * https://oeis.org/A246911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A246911 – numbers m with sigma(m+sigma(m)) = 4*sigma(m);
   Computed by incremental search, caching previously found terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

/* Helper: integer square root, using double sqrt for sufficient precision. */
static uint64_t isqrt_u64(uint64_t n)
{
    return (uint64_t)floor(sqrt((double)n));
}

/* Prime list management */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static uint64_t prime_limit = 0;

static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;
    if (primes)
        free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* σ(n) for an unsigned 64‑bit integer. */
static uint64_t sigma_uint64(uint64_t n)
{
    if (n <= 1)
        return n;   /* σ(0)=0, σ(1)=1 – not used for our search */

    uint64_t result = 1;
    uint64_t m = n;
    uint64_t lim = isqrt_u64(m);
    ensure_primes(lim);

    for (size_t i = 0; i < prime_cnt && primes[i] <= lim; ++i)
    {
        uint64_t p = primes[i];
        if (m % p == 0)
        {
            uint64_t power = 1;
            uint64_t sum_p = 1;
            while (m % p == 0)
            {
                m /= p;
                power *= p;
                sum_p += power;
            }
            /* multiply by the divisor sum for this prime power */
            __uint128_t temp = (__uint128_t)result * sum_p;
            result = (uint64_t)temp;  /* fits in 64 bits for all n in the sequence */
        }
    }

    if (m > 1)  /* remaining prime factor */
    {
        __uint128_t temp = (__uint128_t)result * (1 + m);
        result = (uint64_t)temp;
    }

    return result;
}

/* Cache of discovered terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t last_checked = 0;

/* Append a term to the cache */
static void append_term(seq_t val)
{
    size_t new_cap = cache_len == 0 ? 16 : cache_len * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; guard against 0 */
        return 0;

    while (cache_len < (size_t)n)
    {
        ++last_checked;
        uint64_t m = (uint64_t)last_checked;

        uint64_t sigma_m = sigma_uint64(m);
        uint64_t sum     = m + sigma_m;
        uint64_t sigma_sum = sigma_uint64(sum);

        /* compare using unsigned __int128 to avoid overflow of 4*sigma_m */
        if (sigma_sum == (unsigned __int128)4 * sigma_m)
            append_term((seq_t)m);
    }

    return cache[n - 1];
}
