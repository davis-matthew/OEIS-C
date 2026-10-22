/*
 * A314216  Coordination sequence Gal.6.624.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) * + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) * + 2*a(n-9) - 2*a(n-10) +
 *            2*a(n-11) - a(n-12) * for n > 12, with a(0)=1, a(1)=5, a(2)=11,
 *            a(3)=17, a(4)=22, * a(5)=28, a(6)=33, a(7)=38, a(8)=44, a(9)=49,
 *            a(10)=55, * a(11)=61, a(12)=66
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static size_t cache_cap = 0;

/* Initialise cache with the first 13 terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_cap = 16;                /* at least 13 */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        abort();
    cache[0] = 1;  cache[1] = 5;  cache[2] = 11;
    cache[3] = 17; cache[4] = 22; cache[5] = 28;
    cache[6] = 33; cache[7] = 38; cache[8] = 44;
    cache[9] = 49; cache[10] = 55; cache[11] = 61;
    cache[12] = 66;
    cache_size = 13;
}

/* Expand cache up to index n, computing terms by the recurrence. */
static void ensure_cache(idx_t n)
{
    init_cache();
    if (n < cache_size)
        return;

    size_t needed = n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        /* Compute a(i) by the given recurrence */
        seq_t pos = 0, neg = 0;
        pos += 2 * cache[i - 1];
        pos += 2 * cache[i - 3];
        pos += 2 * cache[i - 5];
        pos += 2 * cache[i - 7];
        pos += 2 * cache[i - 9];
        pos += 2 * cache[i - 11];
        neg += 2 * cache[i - 2];
        neg += 2 * cache[i - 4];
        neg += 2 * cache[i - 6];
        neg += 2 * cache[i - 8];
        neg += 2 * cache[i - 10];
        cache[i] = pos - neg - cache[i - 12];
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
