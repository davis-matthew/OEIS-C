/*
 * A390662  a(n) = Sum_{k=0..floor(2*n/7)} binomial(k+1,2*n-7*k).
 * https://oeis.org/A390662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 126, u64 n <= 246,
 *            u128 n <= 486, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390662.
   a(n) = 2*a(n-4) + a(n-7) - a(n-8),  offset 0.
   Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=2,
                   a(5)=0, a(6)=0, a(7)=1, a(8)=3. */

#include <stdlib.h>
#include <stdint.h>

/* Cached values – grow as needed. */
static seq_t *cache = NULL;
static size_t cache_size = 0;          /* highest allocated index + 1 */

/* Helper to initialize cache for indices 0..8. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_size = 9; /* indices 0..8 */
    cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
    if (cache == NULL)
        abort();   /* out of memory */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)0;
    cache[4] = (seq_t)2;
    cache[5] = (seq_t)0;
    cache[6] = (seq_t)0;
    cache[7] = (seq_t)1;
    cache[8] = (seq_t)3;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    init_cache();

    /* Return cached value if already known. */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Ensure cache large enough for index n. */
    size_t target = (size_t)n;
    size_t old_size = cache_size;
    if (target + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (target + 1) * sizeof(seq_t));
        if (tmp == NULL)
            abort();   /* out of memory */
        for (size_t i = cache_size; i < target + 1; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = target + 1;
    }

    /* Compute new values incrementally using the recurrence. */
    for (size_t i = old_size; i <= target; ++i) {
        if (i >= 8) {                      /* recurrence begins at i = 8 */
            seq_t val = cache[i - 4];
            val += val;                  /* 2 * a(i-4) */
            val += cache[i - 7];          /* + a(i-7) */
            val -= cache[i - 8];          /* - a(i-8) */
            cache[i] = val;
        }
    }

    return cache[n];
}
