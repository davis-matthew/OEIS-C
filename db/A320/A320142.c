/*
 * A320142  Numbers that have exactly two middle divisors.
 * https://oeis.org/A320142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) = the nth integer m such that (# of partitions of m into an odd number of consecutive parts) minus (# of partitions into an even number of consecutive parts) equals 2 */

/* Includes required for the function and its helpers */
#include <stdlib.h>
#include <stdbool.h>

/* Helper to test whether an integer satisfies the partition property. */
static bool satisfies_property(seq_t m)
{
    seq_t twom = m * 2;
    seq_t odd = 0, even = 0;

    /* k is the length of the consecutive parts */
    for (idx_t k = 1;
         (seq_t)k * ((seq_t)k + 1) <= twom;
         ++k)
    {
        /* Only lengths k that divide 2m can give an integer start. */
        if (twom % k != 0)
            continue;

        /* t = 2a = 2m / k - k + 1  must be positive and even. */
        seq_t t = twom / k - (seq_t)k + 1;
        if (t <= 0 || (t & 1) != 0)
            continue;

        /* Count according to parity of k (length). */
        if ((k & 1) != 0)
            odd++;
        else
            even++;
    }

    /* The integer satisfies the property iff difference is 2. */
    return ((long long)odd - (long long)even) == 2;
}

/* Static cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;   /* next integer to test */

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined index, avoid UB */

    /* Extend the cache until it contains n terms. */
    while (cache_len < (size_t)n)
    {
        idx_t cand = next_candidate++;
        seq_t m = (seq_t)cand;

        if (satisfies_property(m))
        {
            /* Grow the cache array when necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
    }

    return cache[n - 1];   /* cache is 0‑based for a(1) */
}
