/*
 * A303183  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 2, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) – 2*a(n-2) + 2*a(n-3) – 3*a(n-4) + a(n-5) (n>9)
 *            Initial values: a(1)=4, a(2)=25, a(3)=31, a(4)=72, a(5)=170,
 *            a(6)=382, a(7)=873, a(8)=2013, a(9)=4618
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <cstddef>      /* size_t */
#else
#include <stddef.h>     /* size_t */
#endif
#include <stdlib.h>     /* malloc, free, realloc */

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static idx_t cache_size = 0;  /* largest n for which cache has a value */

/* Ensure that cache holds values up to and including n */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;                     /* already computed */

    /* Allocate a larger buffer */
    size_t new_alloc = (size_t)n + 1; /* indices 1..n are needed */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_alloc);
    if (!new_cache)
        return;                     /* allocation failure – leave cache unchanged */

    /* Copy existing data */
    idx_t old_size = cache_size;
    for (idx_t i = 1; i <= old_size; ++i)
        new_cache[i] = cache[i];

    /* Release old storage */
    if (cache)
        free(cache);

    cache = new_cache;
    cache_size = n;

    /* Initial values for indices 1..9 (index 0 unused) */
    static const seq_t init_vals[10] = {
        0,          /* unused */
        (seq_t)4,   /* a(1) */
        (seq_t)25,  /* a(2) */
        (seq_t)31,  /* a(3) */
        (seq_t)72,  /* a(4) */
        (seq_t)170, /* a(5) */
        (seq_t)382, /* a(6) */
        (seq_t)873, /* a(7) */
        (seq_t)2013,/* a(8) */
        (seq_t)4618 /* a(9) */
    };

    /* Compute any new entries */
    for (idx_t i = old_size + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init_vals[i];
        } else {
            /* a(i) = 3*a(i‑1) – 2*a(i‑2) + 2*a(i‑3) – 3*a(i‑4) + a(i‑5) */
            seq_t t = cache[i-1] * 3;
            t += cache[i-3] * 2;
            t += cache[i-5];
            t -= cache[i-2] * 2;
            t -= cache[i-4] * 3;
            cache[i] = t;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n=0, but safe to return 0 */

    ensure_cache(n);
    return cache[n];
}
