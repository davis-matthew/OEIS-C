/*
 * A291915  Number of 6-cycles in the n-Fibonacci cube graph.
 * https://oeis.org/A291915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 5*a(n-4) + 8*a(n-5) -
 *            2*a(n-6) - 4*a(n-7) - a(n-8) (a(1)=0,a(2)=0,a(3)=0,a(4)=2,
 *            a(5)=22,a(6)=82,a(7)=268,a(8)=742)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 30, u64 n <= 71, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed terms.  Index 0 is unused. */
static seq_t *cache = NULL;
static idx_t  max_computed = 0;   /* highest n already computed */
static size_t cache_capacity = 0; /* allocated slots (including index 0) */

/* Ensure that the cache has room for indices up to needed. */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed + 1 <= cache_capacity)
        return;
    size_t new_capacity = (needed + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp) abort();          /* memory failure */
    cache = tmp;
    /* new slots are zero-initialised by realloc (implementation-defined), but
       we don't rely on that value. They will be overwritten before use. */
    cache_capacity = new_capacity;
}

/* Initialize base terms if this is the first call. */
static void init_cache(void)
{
    /* Need indices 1..8 */
    ensure_cache_capacity(8);
    cache[1] = 0;
    cache[2] = 0;
    cache[3] = 0;
    cache[4] = 2;
    cache[5] = 22;
    cache[6] = 82;
    cache[7] = 268;
    cache[8] = 742;
    max_computed = 8;
}

seq_t a(idx_t n)
{
    if (cache == NULL)
        init_cache();

    if (n <= max_computed)
        return cache[n];

    /* compute missing terms up to n */
    ensure_cache_capacity(n);
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* Compute using positive and negative parts to avoid unsigned underflow */
        seq_t pos = 4 * cache[i - 1];
        pos += 5 * cache[i - 4];
        pos += 8 * cache[i - 5];

        seq_t neg = 2 * cache[i - 2];
        neg += 8 * cache[i - 3];
        neg += 2 * cache[i - 6];
        neg += 4 * cache[i - 7];
        neg += cache[i - 8];

        cache[i] = pos - neg;
    }
    max_computed = n;
    return cache[n];
}
