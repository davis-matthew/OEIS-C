/*
 * A295731  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -1,
 *          a(1) = -1, a(2) = 0, a(3) = 1.
 * https://oeis.org/A295731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 42, i64 n <= 88, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+3*a(n-2)-2*a(n-3)-2*a(n-4),
   a(0)=-1, a(1)=-1, a(2)=0, a(3)=1 */
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;     /* current capacity of the cache */
static idx_t cache_size = 0;     /* number of terms actually stored */

/* Ensure that the cache can hold indices up to 'needed' inclusive. */
static void ensure_capacity(idx_t needed)
{
    if (needed + 1 > (idx_t)cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < needed + 1)
            new_cap = needed + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(1); /* allocation failure */
        cache = new_cache;
        cache_cap = (size_t)new_cap;
    }
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Lazy initialization of the cache with the first four values */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache_size = 4;
    }

    if (n < cache_size)
        return cache[n];

    /* Compute terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        ensure_capacity(i);
        cache[i] = cache[i-1] + (seq_t)3*cache[i-2]
                     - (seq_t)2*cache[i-3] - (seq_t)2*cache[i-4];
    }
    cache_size = n + 1;
    return cache[n];
}
