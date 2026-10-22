/*
 * A348623  a(1) = 1; for n > 1 a(n) = a(n-1) + A001227(a(n-1)).
 * https://oeis.org/A348623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) + A001227(a(n-1)), where
 *            A001227(k) = number of odd divisors of k
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: count odd divisors of x.  */
static seq_t odd_divisor_count(seq_t x)
{
    /* Remove factors of 2. */
    while ((x & (seq_t)1) == 0)
        x >>= 1;

    seq_t count = 1;
    /* Count divisors of the remaining odd part. */
    for (seq_t p = 3; p <= x / p; p += 2)
    {
        if (x % p == 0)
        {
            seq_t exp = 0;
            while (x % p == 0)
            {
                x /= p;
                exp++;
            }
            count *= (exp + 1);
        }
    }
    if (x > 1)                /* remaining prime factor */
        count *= 2;           /* exp = 1 -> (1+1) = 2  */
    return count;
}

/* Memoization table for the sequence.  */
static seq_t *seq_cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range, return 0 (never used) */

    /* Ensure the cache covers index n. */
    if (seq_cache == NULL)
    {
        cache_cap = n + 1;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;
        seq_cache[1] = (seq_t)1;          /* a(1) = 1 */
    }
    else if (cache_cap <= n)
    {
        cache_cap = n + 1;
        seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t prev = seq_cache[i - 1];
        seq_t inc  = odd_divisor_count(prev);
        seq_cache[i] = prev + inc;
    }
    cache_len = n > cache_len ? n : cache_len;

    return seq_cache[n];
}
