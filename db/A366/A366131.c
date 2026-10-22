/*
 * A366131  Number of subsets of {1..n} with two elements (possibly the same)
 *          summing to n.
 * https://oeis.org/A366131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 6*a(n-3) for n>3, with a(0)=0,
 *            a(1)=0, a(2)=2, a(3)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_max = 0;
static bool initialized = false;

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    /* Allocate more memory (at least needed elements) */
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    cache = new_cache;
    cache_cap = needed;
}

static void init_cache(void)
{
    /* Base case values a(0)=0, a(1)=0, a(2)=2, a(3)=2 */
    ensure_capacity(4);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)2;
    cache_max = 3;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_cache();

    if (n <= cache_max)
        return cache[n];

    /* Expand cache to cover n */
    ensure_capacity(n + 1);

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        /* a(i) = 2*a(i-1) + 3*a(i-2) - 6*a(i-3) */
        cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] - 6 * cache[i - 3];
    }
    cache_max = n;
    return cache[n];
}
