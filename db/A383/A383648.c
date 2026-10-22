/*
 * A383648  Expansion of 1/((1-x) * (1+3*x) * (1-5*x) * (1+7*x) * (1-9*x)).
 * https://oeis.org/A383648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 70*a(n-2) - 230*a(n-3) - 789*a(n-4) +
 *            945*a(n-5) initial values: a(0)=1, a(1)=5, a(2)=95, a(3)=595,
 *            a(4)=7686
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated elements */
    static idx_t last_computed = (idx_t)-1; /* last index stored */

    /* initial terms */
    const seq_t init[5] = {(seq_t)1,
                           (seq_t)5,
                           (seq_t)95,
                           (seq_t)595,
                           (seq_t)7686};

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_size = 6; /* allocate room for up to index 5 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        /* reserve one extra slot for convenience */
        cache[5] = 0;
        last_computed = 4;
    }

    /* if requested n is already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* extend cache as needed */
    if (n >= cache_size) {
        /* double the capacity until we can hold index n */
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute terms from last_computed+1 up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t t1 = cache[k-1] * (seq_t)5;
        seq_t t2 = cache[k-2] * (seq_t)70;
        seq_t t3 = cache[k-3] * (seq_t)230;
        seq_t t4 = cache[k-4] * (seq_t)789;
        seq_t t5 = cache[k-5] * (seq_t)945;

        seq_t val = t1 + t2;
        val += t5;
        val -= t3;
        val -= t4;

        cache[k] = val;
    }
    last_computed = n;
    return cache[n];
}
