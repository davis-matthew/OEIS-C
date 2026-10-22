/*
 * A372984  a(n) = floor((a(n-1)+a(n-2)+n)/(a(n-3)+1)), with a(1)=1, a(2)=3,
 *          a(3)=4.
 * https://oeis.org/A372984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((a(n‑1)+a(n‑2)+n)/(a(n‑3)+1)), a(1)=1, a(2)=3,
 *            a(3)=4
 * Range:     u8 n <= 306, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache to memoize already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated capacity of cache */
    static size_t computed = 0;     /* number of terms already computed (>=3 after init) */

    /* Return directly for the first three terms. */
    if (n <= 3) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)3;
            case 3: return (seq_t)4;
        }
    }

    /* If we need more terms than currently computed, compute them. */
    if ((size_t)n > computed) {
        /* Allocate or grow the cache as needed. */
        if (cache_size < (size_t)n) {
            size_t new_size = (cache_size == 0) ? 8 : cache_size;
            while (new_size < (size_t)n) new_size <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failure – safe fallback */
            cache = tmp;
            cache_size = new_size;
        }

        /* Initialise base values if this is the first time. */
        if (computed == 0) {
            cache[0] = (seq_t)1;   /* a(1) */
            cache[1] = (seq_t)3;   /* a(2) */
            cache[2] = (seq_t)4;   /* a(3) */
            computed = 3;
        }

        /* Compute terms from computed+1 up to n. */
        for (size_t i = computed + 1; i <= (size_t)n; ++i) {
            /* i corresponds to a(i). Cache index is i-1. */
            seq_t numerator   = cache[i-2] + cache[i-3] + (seq_t)i;
            seq_t denominator = cache[i-4] + (seq_t)1;
            cache[i-1] = numerator / denominator;   /* floor division */
        }
        computed = n;
    }

    return cache[(size_t)(n-1)];
}
