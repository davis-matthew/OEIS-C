/*
 * A254966  Heptagonal numbers (A000566) that are also centered hexagonal
 *          numbers (A003215).
 * https://oeis.org/A254966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1)+482*a(n‑2)-482*a(n‑3)-a(n‑4)+a(n‑5) with initial
 *            values a(1)=1, a(2)=7, a(3)=469, a(4)=3367, a(5)=226051 (OEIS
 *            A254966)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */
#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] holds a(1) */
    static idx_t  cached = 0;     /* number of terms already computed */
    static size_t cap    = 0;     /* allocated capacity of cache */

    /* Initialise cache with the first five terms on first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed (unlikely here) */
        cap   = 5;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)469;
        cache[3] = (seq_t)3367;
        cache[4] = (seq_t)226051;
        cached = 5;
    }

    /* Grow the array if necessary to hold up to term n. */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) return 0;   /* allocation failed */
        cache = p;
        cap   = new_cap;
    }

    /* Return a known value if already calculated. */
    if (n <= cached) {
        return cache[n - 1];
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* i >= 6 ensures indices i-6 >= 0. */
        seq_t val = cache[i - 2]                    /* a(i‑1) */
                    + (seq_t)482 * cache[i - 3]     /* +482 * a(i‑2) */
                    - (seq_t)482 * cache[i - 4]     /* -482 * a(i‑3) */
                    - cache[i - 5]                 /* -a(i‑4) */
                    + cache[i - 6];                /* +a(i‑5) */
        cache[i - 1] = val; /* store a(i) */
    }

    cached = n;
    return cache[n - 1];
}
