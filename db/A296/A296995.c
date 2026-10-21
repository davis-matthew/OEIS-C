/*
 * A296995  Number of edge covers in the n-dipyramidal graph.
 * https://oeis.org/A296995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 24*a(n-2) - 21*a(n-3) + 33*a(n-4) + 34*a(n-5)
 *            + 8*a(n-6) for n>6; a(1)=2, a(2)=34, a(3)=341, a(4)=2902,
 *            a(5)=23092, a(6)=178393
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but 0 is harmless */

    /* Allocate and initialise static cache on first call. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* capacity of cache array */
    static idx_t last_computed = 0;   /* highest n for which cache[n] is valid */

    /* initial values (a[1]..a[6]), dummy a[0] = 0 */
    static const seq_t initvals[7] = {
        (seq_t)0,
        (seq_t)2,
        (seq_t)34,
        (seq_t)341,
        (seq_t)2902,
        (seq_t)23092,
        (seq_t)178393
    };

    /* Ensure capacity and initialise first six entries. */
    if (cache == NULL) {
        /* start with a small power of two, grow as needed */
        cache_size = 64;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed – return 0 */

        /* initialise indices 0..6 */
        for (idx_t i = 0; i <= 6 && i < cache_size; ++i)
            cache[i] = initvals[i];
        last_computed = 6;
    } else if (cache_size <= 6) {
        /* This case only on extreme low allocations; ensure it can hold indices 0..6. */
        cache_size = 64;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        for (idx_t i = 0; i <= 6; ++i)
            cache[i] = initvals[i];
        last_computed = 6;
    }

    /* Grow cache if necessary. */
    if ((idx_t)n >= cache_size) {
        idx_t new_cap = cache_size;
        while ((idx_t)n >= new_cap) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_size = new_cap;
    }

    /* Compute any missing terms up to n. */
    while (last_computed < n) {
        idx_t i = last_computed + 1;
        /* recurrence uses terms i-1 .. i-6 */
        seq_t sum_plus  = (seq_t)11 * cache[i-1];
        sum_plus += (seq_t)33 * cache[i-4];
        sum_plus += (seq_t)34 * cache[i-5];
        sum_plus += (seq_t)8  * cache[i-6];

        seq_t sum_minus = (seq_t)24 * cache[i-2];
        sum_minus += (seq_t)21 * cache[i-3];

        cache[i] = sum_plus - sum_minus;  /* valid because result is non‑negative */
        last_computed = i;
    }

    return cache[n];
}
