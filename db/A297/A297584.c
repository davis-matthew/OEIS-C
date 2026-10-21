/*
 * A297584  Number of 3 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 or 4 neighboring 1's.
 * https://oeis.org/A297584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 8*a(n-3) + 18*a(n-4) + a(n-5) -
 *            11*a(n-6) - 12*a(n-7) - a(n-8) Initial values: a(1)=1, a(2)=9,
 *            a(3)=36, a(4)=72, a(5)=188, a(6)=688, a(7)=2085, a(8)=5497
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <assert.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity */
    static idx_t computed = 0;  /* largest n whose value is stored in cache */

    /* First eight terms of the sequence */
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)9,
        (seq_t)36,
        (seq_t)72,
        (seq_t)188,
        (seq_t)688,
        (seq_t)2085,
        (seq_t)5497
    };

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cap = 8;
        cache = alloc_seq_array(cap);
        assert(cache != NULL);
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        computed = 8;
    }

    /* If the request is within the first eight terms, return immediately */
    if (n <= 8)
        return init[n - 1];

    /* Grow the cache if needed */
    if ((idx_t)cap < n) {
        grow_seq(&cache, &cap, n);
        assert(cache != NULL);
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 2]                       /* a(i-1) */
            + cache[i - 3]                            /* a(i-2) */
            + (seq_t)8 * cache[i - 4]                 /* 8*a(i-3) */
            + (seq_t)18 * cache[i - 5]                /* 18*a(i-4) */
            + cache[i - 6]                            /* a(i-5) */
            - (seq_t)11 * cache[i - 7]                /* 11*a(i-6) */
            - (seq_t)12 * cache[i - 8]                /* 12*a(i-7) */
            - cache[i - 9];                           /* a(i-8) */
        cache[i - 1] = val;
    }
    computed = n;

    return cache[n - 1];
}
