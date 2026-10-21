/*
 * A249997  Expansion of 1/((1-x)*(1+3*x)*(1-4*x)).
 * https://oeis.org/A249997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+11*a(n-2)-12*a(n-3) for n ≥ 3, a(0)=1, a(1)=2,
 *            a(2)=15 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>   /* malloc, free, abort */

static seq_t *cache = NULL;
static size_t cache_size = 0;     /* number of values currently stored */
static size_t cache_capacity = 0; /* allocated size of cache */

/* Ensure that the cache can hold at least 'req' elements.  */
static void ensure_capacity(size_t req)
{
    if (req <= cache_capacity)
        return;

    size_t new_cap = cache_capacity ? cache_capacity : 1;
    while (new_cap < req)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
#else
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!tmp)
        abort();

    if (cache) {
#ifdef __cplusplus
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        delete [] cache;
#else
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        free(cache);
#endif
    }

    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    size_t req = (size_t)n + 1;
    if (n < 3) {
        /* Fast path for the first three terms */
        switch ((int)n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)15;
        default:
            break; /* should not happen */
        }
    }

    ensure_capacity(req);

    for (size_t i = cache_size; i < req; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)2;
        else if (i == 2)
            cache[i] = (seq_t)15;
        else
            cache[i] = ( (seq_t)2 * cache[i - 1] )
                     + ( (seq_t)11 * cache[i - 2] )
                     - ( (seq_t)12 * cache[i - 3] );
    }

    cache_size = req;
    return cache[n];
}
