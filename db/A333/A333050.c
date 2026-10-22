/*
 * A333050  a(1) = 1; a(n+1) = Sum_{d|n, gcd(d, n/d) = 1} a(d).
 * https://oeis.org/A333050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 174, u32 n <= 1438, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333050: a(1)=1; a(n+1)=sum_{d|n, gcd(d,n/d)=1} a(d) */

#include <stddef.h>
#include <stdlib.h>

/* Euclidean algorithm for unsigned 64‑bit integers (used for the coprimality test). */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cached sequence values.  cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;    /* number of valid entries in cache */
static idx_t cache_cap = 0;    /* allocated capacity */

/* Ensure that the cache has room for at least n values. */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
    {
        /* In the unlikely event of a memory allocation failure, abort. */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n=0, return 0 as a sentinel */

    /* Fill cache up to n. */
    if (n > cache_len)
    {
        /* Ensure capacity first. */
        ensure_cache_size(n);

        /* Compute missing values. */
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            if (i == 1)   /* a(1) = 1 */
            {
                cache[0] = (seq_t)1;
                continue;
            }

            idx_t m = i - 1;
            seq_t sum = 0;

            /* Enumerate divisors of m. */
            for (idx_t d = 1; d * d <= m; ++d)
            {
                if (m % d == 0)
                {
                    idx_t q = m / d;

                    /* Divisor d. */
                    if (gcd_uint64(d, q) == 1)
                        sum += cache[d - 1];

                    /* Complementary divisor q, avoid double counting when d==q. */
                    if (d != q && gcd_uint64(q, d) == 1)
                        sum += cache[q - 1];
                }
            }

            cache[i - 1] = sum;
        }

        cache_len = n;
    }

    return cache[n - 1];
}
