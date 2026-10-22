/*
 * A306756  a(n) = a(n-7) + a(n-8) with a(0)=8, a(1)=...=a(6)=0, a(7)=7.
 * https://oeis.org/A306756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 118, u32 n <= 238, u64 n <= 479,
 *            u128 n <= 958, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-7) + a(n-8) in OEIS (offset 0)  
   Initial values: a(0)=8, a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=7. */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed terms            */
static idx_t  cache_size = 0;        /* number of terms stored in cache    */
static idx_t  cache_cap   = 0;        /* current array capacity             */

/* Seed the cache with the first eight terms. */
static void init_cache(void)
{
    /* Ensure at least capacity for the initial eight terms */
    idx_t init_cap = 8;
    if (cache_cap < init_cap) {
        cache_cap = init_cap;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return;      /* allocation failure – caller will handle */
    }

    /* Set the known initial values */
    cache[0] = 8;
    cache[1] = cache[2] = cache[3] = cache[4] = cache[5] = cache[6] = 0;
    cache[7] = 7;

    cache_size = 8;                /* we have computed the first 8 terms */
}

/* Grow the cache until it contains at least term n. */
static void ensure_cache(idx_t n)
{
    if (cache_size == 0) init_cache();

    /* Expand the cache as necessary */
    while (cache_size <= n) {
        /* Ensure there is enough allocated space */
        if (cache_size >= cache_cap) {
            /* Double the capacity */
            idx_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) break;          /* allocation failure – stop growing   */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute the next term via the recurrence */
        seq_t val = cache[cache_size - 7] + cache[cache_size - 8];
        cache[cache_size] = val;
        ++cache_size;
    }
}

/* Public function returning the nth term of the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
