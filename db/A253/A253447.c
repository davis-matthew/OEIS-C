/*
 * A253447  Indices of centered octagonal numbers (A016754) which are also
 *          centered heptagonal numbers (A069099).
 * https://oeis.org/A253447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 31*a(n-2) + a(n-3) with a(1)=1, a(2)=15,
 *            a(3)=435
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* array of already computed terms   */
static size_t cache_size = 0;       /* number of elements stored         */
static size_t cache_capacity = 0;   /* current allocated capacity        */

/* Return the nth term of A253447 (1‑based index). */
seq_t a(idx_t n)
{
    /* n must be at least 1 (offset 1).  Undefined behaviour for n<1, return 0. */
    if (n < 1) return (seq_t)0;

    /* Initialise cache with the first three terms. */
    if (!cache) {
        cache_capacity = 8;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failed */

        cache[0] = (seq_t)1;      /* a(1) */
        cache[1] = (seq_t)15;     /* a(2) */
        cache[2] = (seq_t)435;    /* a(3) */
        cache_size = 3;
    }

    /* If n is within the already cached range, return directly. */
    if ((size_t)n <= cache_size)
        return cache[n-1];

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_capacity) {
        size_t new_cap = cache_capacity;
        while (new_cap < (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* reallocation failed   */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        /* i  is zero‑based index; current term is a(i+1) */
        seq_t term = (seq_t)31 * cache[i-1]   /* a(i)   */
                   - (seq_t)31 * cache[i-2]   /* a(i-1) */
                   + cache[i-3];             /* a(i-2) */
        cache[i] = term;
    }
    cache_size = (size_t)n;

    return cache[n-1];
}
