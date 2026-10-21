/*
 * A298915  Number of nX6 0..1 arrays with every element equal to 0, 3, 5, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) - 2*a(n-4) - a(n-7) +
 *            2*a(n-8) for n > 9 Initial values: a(1) = 1, a(2) = 5, a(3) = 5,
 *            a(4) = 6, a(5) = 9, a(6) = 14, a(7) = 24, a(8) = 44, a(9) = 81
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated length (number of slots) */
    static idx_t max_n  = 0;          /* highest n computed so far */

    /* initial terms (index 0 unused) */
    static const seq_t init[10] = {
        (seq_t)0,   /* dummy for 0‑based indexing */
        (seq_t)1,   /* a(1) */
        (seq_t)5,   /* a(2) */
        (seq_t)5,   /* a(3) */
        (seq_t)6,   /* a(4) */
        (seq_t)9,   /* a(5) */
        (seq_t)14,  /* a(6) */
        (seq_t)24,  /* a(7) */
        (seq_t)44,  /* a(8) */
        (seq_t)81   /* a(9) */
    };

    /* guard against invalid (n < 1) */
    if (n < 1)
        return (seq_t)0;

    /* allocate cache on first call */
    if (!cache) {
        cap = 64;  /* initial capacity, will grow as needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failed – unlikely */

        /* initialise known terms */
        for (idx_t i = 1; i <= 9 && i < cap; ++i)
            cache[i] = init[i];
        max_n = 9;
    }

    /* return cached value if already computed */
    if (n <= max_n)
        return cache[n];

    /* enlarge cache until it can hold index n */
    while (cap <= n) {
        size_t newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            return (seq_t)0;
        }
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        seq_t val = ((seq_t)3 * cache[cur-1]
                     - (seq_t)2 * cache[cur-2])    /* 3*a(n-1) - 2*a(n-2) */
                    +  cache[cur-3]                  /* + a(n-3) */
                    - (seq_t)2 * cache[cur-4]        /* - 2*a(n-4) */
                    -  cache[cur-7]                  /* - a(n-7) */
                    + (seq_t)2 * cache[cur-8];       /* + 2*a(n-8) */
        cache[cur] = val;
    }
    max_n = n;
    return cache[n];
}
