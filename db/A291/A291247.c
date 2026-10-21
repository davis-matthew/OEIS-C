/*
 * A291247  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^2 - S^3 +
 *          S^4.
 * https://oeis.org/A291247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2) - 2*a(n-3) - 9*a(n-4) + 2*a(n-5) +
 *            5*a(n-6) - a(n-7) - a(n-8), with initial values a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=10, a(4)=24, a(5)=49, a(6)=112, a(7)=238, a(8)=526
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* initial values for n < 9 */
    static const seq_t init[9] = {1, 2, 5, 10, 24, 49, 112, 238, 526};
    if (n < 9) return init[n];

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static idx_t cache_size = 0;   /* number of entries already computed */

    /* allocate and initialise cache if needed */
    if (cache == NULL) {
        cache_capacity = 32;                   /* starting capacity > 9 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        /* copy initial values */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_size = 9;
    }

    /* grow cache if request exceeds current capacity */
    if ((size_t)n >= cache_capacity) {
        size_t new_cap = cache_capacity;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_capacity = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* compute recurrence using only additions / subtractions
           to avoid negative intermediate values */
        seq_t pos = cache[i-1];
        pos += 5 * cache[i-2];
        pos += 2 * cache[i-5];
        pos += 5 * cache[i-6];

        seq_t neg = 2 * cache[i-3];
        neg += 9 * cache[i-4];
        neg += cache[i-7];
        neg += cache[i-8];

        cache[i] = pos - neg;  /* result is guaranteed non‑negative */
    }
    cache_size = (idx_t)((size_t)n + 1);

    return cache[n];
}
