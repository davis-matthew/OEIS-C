/*
 * A333211  Variation of Van Eck's sequence A181391: a(n+1) = the minimum
 *          positive offset m from a(n) such that a(n-m-1)*a(n-m) =
 *          a(n-1)*a(n); a(n+1)=0 if no such m exists. Start with a(1) = a(2)
 *          = 0.
 * https://oeis.org/A333211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 0; a(n+1) = the minimum positive integer m such
 *            that a(n-m-1) * a(n-m) = a(n-1) * a(n); a(n+1)=0 if no such m
 *            exists. (This is OEIS A333211.)
 * Range:     u8 n <= 113, u16 n <= 5359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* sequence, 1‑indexed (cache[0] unused) */
static idx_t  cache_size = 0;        /* largest computed index */
static size_t cache_cap  = 0;         /* allocated capacity (>= cache_size) */

/* Make sure cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = (size_t)n;
    /* cast the void* returned by realloc to seq_t* */
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    /* initialise newly allocated slots to zero */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 1; memoises already‑calculated values. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for non‑positive indices */
    ensure_capacity(n);
    if (n <= cache_size) return cache[n];

    /* Compute terms from cache_size+1 up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        if (i == 1 || i == 2)
        {
            cache[i] = 0;
        }
        else
        {
            seq_t target = cache[i-2] * cache[i-1];   /* a(i-2)*a(i-1) */
            seq_t value = 0;
            for (idx_t m = 1; m <= i-3; ++m)
            {
                seq_t candidate = cache[i-m-2] * cache[i-m-1];
                if (candidate == target)
                {
                    value = m;
                    break;          /* minimum m found */
                }
            }
            cache[i] = value;
        }
    }
    cache_size = n;
    return cache[n];
}
