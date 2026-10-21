/*
 * A287825  Number of sequences over the alphabet {0,1,...,9} such that no two
 *          consecutive terms have distance 1.
 * https://oeis.org/A287825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 4*a(n-2) - 21*a(n-3) + 9*a(n-4) + 5*a(n-5),
 *            a(0)=1, a(1)=10, a(2)=82, a(3)=674, a(4)=5540, a(5)=45538
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t size = 0;  /* number of values stored (cache[0 .. size-1]) */

    /* first six values are known constants */
    static const seq_t init[6] = {1, 10, 82, 674, 5540, 45538};

    /* fast path for the first six indices */
    if (n < 6) return init[(size_t)n];

    /* initialise the cache on first use */
    if (cache == NULL) {
        size_t init_cap = 6;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();          /* out‑of‑memory */
        for (size_t i = 0; i < init_cap; ++i) cache[i] = init[i];
        cap  = init_cap;
        size = init_cap;              /* we have values 0 .. 5 stored */
    }

    /* grow the cache to contain up to index n */
    if ((size_t)n >= size) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 6;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory */
        cache = tmp;
        cap = new_cap;

        /* compute all new terms up to n */
        for (size_t i = size; i <= (size_t)n; ++i) {
            /* compute the linear combination without using negative intermediate */
            seq_t pos = 9 * cache[i-1]  /* 9*a(n-1) */
                      + 9 * cache[i-4]  /* + 9*a(n-4) */
                      + 5 * cache[i-5]; /* + 5*a(n-5) */

            seq_t neg = 4 * cache[i-2]  /* + 4*a(n-2) */
                      + 21 * cache[i-3];/* + 21*a(n-3) */

            cache[i] = pos - neg;      /* pos >= neg in the sequence */
        }
        size = (size_t)n + 1; /* now we have values up to n */
    }

    return cache[(size_t)n];
}
