/*
 * A269037  Number of 3 X n 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 28*a(n-3) - 4*a(n-4) + 16*a(n-5) -
 *            4*a(n-6) for n > 8. Initial values: a(1)=0, a(2)=120, a(3)=348,
 *            a(4)=2166, a(5)=9528, a(6)=44760, a(7)=198816, a(8)=877800
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Static tables for memoisation. */
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t  max_computed = 0;      /* highest index that has a value */
    static idx_t  capacity = 0;          /* current allocated size (>= max_computed) */
    static int   init = 0;               /* initialisation flag */

    /* Initialise the table with the first eight terms. */
    if (!init) {
        capacity = 8;
        cache = (seq_t*)malloc(sizeof(seq_t) * (capacity + 1)); /* +1 to use 1‑based indexing */
        if (!cache) abort();

        /* Base values (index 0 is unused). */
        const seq_t base[9] = {
            (seq_t)0,   /* placeholder for index 0 */
            (seq_t)0,   /* a(1) */
            (seq_t)120, /* a(2) */
            (seq_t)348, /* a(3) */
            (seq_t)2166,/* a(4) */
            (seq_t)9528,/* a(5) */
            (seq_t)44760,/* a(6) */
            (seq_t)198816,/* a(7) */
            (seq_t)877800 /* a(8) */
        };
        for (idx_t i = 1; i <= capacity; ++i)
            cache[i] = base[i];

        max_computed = capacity;
        init = 1;
    }

    /* Return immediately if we already know the value. */
    if (n <= max_computed)
        return cache[n];

    /* Extend the array if necessary. */
    if (n > capacity) {
        capacity = n;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * (capacity + 1));
        if (!cache) abort();
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The recurrence is valid for i > 8; the first eight are known. */
        cache[i] = (seq_t)6 * cache[i-1]
                 - cache[i-2]
                 - (seq_t)28 * cache[i-3]
                 - (seq_t)4  * cache[i-4]
                 + (seq_t)16 * cache[i-5]
                 - (seq_t)4  * cache[i-6];
    }

    max_computed = n;
    return cache[n];
}
