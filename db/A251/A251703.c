/*
 * A251703  4-step Fibonacci sequence starting with 1,1,0,0.
 * https://oeis.org/A251703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n)+a(n+1)+a(n+2)+a(n+3)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=0 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/* static cache for the computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;          /* allocated entries */
static idx_t  cache_upto = 0;          /* highest index that is defined */

/* ensure that terms up to index n are present in the cache */
static void
ensure_cache(idx_t n)
{
    /* initialise cache once */
    if (cache == NULL) {
        /* seed the first four values */
        cache_size = 4;
        cache_upto = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();      /* allocation failure */
        cache[0] = (seq_t)1;      /* a(0) */
        cache[1] = (seq_t)1;      /* a(1) */
        cache[2] = (seq_t)0;      /* a(2) */
        cache[3] = (seq_t)0;      /* a(3) */
    }

    /* nothing to do if we already have this value */
    if (n < cache_upto)
        return;

    /* grow the allocated storage if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size ? cache_size : 1;
        while ((size_t)(n + 1) > new_size)
            new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute the missing terms */
    for (idx_t k = cache_upto + 1; k <= n; ++k) {
        /* cache[k-1] .. cache[k-4] are already defined */
        cache[k] = cache[k - 1] + cache[k - 2] + cache[k - 3] + cache[k - 4];
    }

    cache_upto = n;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
