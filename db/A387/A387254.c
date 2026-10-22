/*
 * A387254  Squares of numbers that are neither prime powers nor powerful.
 * https://oeis.org/A387254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A387254:  squares of integers that are neither prime powers nor powerful.
    Implementation:  iterate over integers k ≥ 2, test k for the property
    (at least two distinct prime factors and at least one exponent equal to 1),
    collect its square, and cache the results for fast future look‑ups. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* small helper: test whether n has at least two distinct prime factors
   and not all exponents are ≥2 (i.e. it is not a prime power and not powerful). */
static bool is_allowed(uint64_t n)
{
    if (n < 2)
        return false;

    uint64_t m = n;
    int    distinct = 0;
    bool   hasExp1   = false;

    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            ++distinct;
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            if (exp == 1)
                hasExp1 = true;
        }
    }

    if (m > 1)        /* remaining prime factor with exponent 1 */
    {
        ++distinct;
        hasExp1 = true;
    }

    return distinct >= 2 && hasExp1;
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached sequence values */
    static idx_t  cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* capacity of cache array */
    static uint64_t last_k = 1;    /* last integer tested */

    if (n == 0)
        return 0;                 /* offset starts at 1 */

    /* grow cache until it contains at least n terms */
    if (cache_len < n)
    {
        if (!cache)
        {
            cache_cap = 1024;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        }

        for (uint64_t k = last_k + 1; /* infinite, break inside */ ; ++k)
        {
            if (is_allowed(k))
            {
                if (cache_len == cache_cap)
                {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = (seq_t)k * (seq_t)k;
                if (cache_len == n)
                {
                    last_k = k;
                    break;
                }
            }
            last_k = k;
        }
    }

    return cache[n - 1];
}
