/*
 * A342636  a(0) = 0, a(1) = 1; a(2*n) = a(n), a(2*n+1) = 7*a(n) + a(n+1).
 * https://oeis.org/A342636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; a(2*n)=a(n), a(2*n+1)=7*a(n)+a(n+1)
 * Range:     u8 n <= 14, u16 n <= 126, u32 n <= 6142, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_size = 0; /* number of values computed so far */

/* Computes a(n) for OEIS sequence A342636 */
seq_t a(idx_t n)
{
    /* Ensure enough space in the cache */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = n + 1;
        cache_size = 0;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (inclusive) */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if ((i & 1) == 0) {
            /* i = 2 * (i/2) */
            cache[i] = cache[i >> 1];
        } else {
            /* i = 2 * (i/2) + 1 */
            idx_t u = i >> 1;          /* floor division by 2 */
            seq_t term = cache[u];
            cache[i] = (seq_t)7 * term + cache[u + 1];
        }
    }
    cache_size = n + 1;

    return cache[n];
}
