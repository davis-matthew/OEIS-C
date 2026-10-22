/*
 * A349836  Expansion of Sum_{k>=0} (k * x)^k/(1 - k^2 * x).
 * https://oeis.org/A349836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} k^(2*n-k) (OEIS A349836)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

/* Recursive computation of natural numbers.
   Memoises all needed values so that calls are O(max(n)). */
seq_t a(idx_t n) {
    /* static storage for computed values */
    static seq_t *T = NULL;            /* temporary values k^(2*i-k) for current i */
    static seq_t *cache = NULL;        /* a(i) for all computed i */
    static idx_t cache_size = 0;       /* allocated size of T and cache arrays  */
    static idx_t max_n = 0;            /* highest n that has been computed  */

    /* Expand storage if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        new_size = new_size * 2;      /* over‑allocate to reduce reallocs */
        T = (seq_t *)realloc(T, new_size * sizeof(seq_t));
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = (idx_t)new_size;
    }

    /* Initialise for n==0 if this is the first call */
    if (max_n == 0 && cache[0] == 0) {
        T[0] = 1;
        cache[0] = 1;
        max_n = 0;
    }

    /* Incrementally compute a(i) up to the requested n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* Update existing terms k^(2*i-k) = k^2 * previous term */
        for (idx_t k = 0; k < i; ++k) {
            seq_t k2 = (seq_t)k * (seq_t)k;
            T[k] = T[k] * k2;
            sum += T[k];
        }

        /* Add the new term for k == i : i^i */
        T[i] = big_pow((seq_t)i, (seq_t)i);
        sum += T[i];

        /* Store the result */
        cache[i] = sum;
    }

    max_n = n;
    return cache[n];
}
