/*
 * A298413  a(n) = a(floor(n/2))*a(ceiling(n/2)), where a(0) = 1, a(1) = 2,
 *          a(2) = 3.
 * https://oeis.org/A298413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(floor(n/2))*a(ceil(n/2)), a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 77, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>       // For malloc, realloc

static seq_t *cache = NULL;   /* dynamic array of computed values     */
static size_t cache_cap = 0;  /* allocated buffer size                 */
static size_t computed = 0;   /* number of values already initialized */

static void ensure_cache_size(idx_t n)
{
    /* Allocate or grow the cache if necessary, and initialize the base cases. */
    if (cache == NULL) {
        size_t init = (n + 1 > 3 ? n + 1 : 3);
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (cache == NULL) exit(1);          /* out of memory */
        cache_cap = init;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        computed = 3;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* Compute any values that have not yet been produced. */
    for (idx_t i = (idx_t)computed; i <= n; ++i) {
        if (i <= 2) continue;                /* base cases already set */
        idx_t left  = i >> 1;                 /* floor(i/2) */
        idx_t right = (i + 1) >> 1;           /* ceil(i/2)  */
        cache[i] = cache[left] * cache[right];
    }
    computed = (size_t)n + 1;
    return cache[n];
}
