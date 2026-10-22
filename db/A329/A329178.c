/*
 * A329178  Sum of the products of pairs of Padovan numbers which are two
 *          apart, starting from A000931(5).
 * https://oeis.org/A329178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=5..n+5} A000931(i)*A000931(i+2). Recurrence: a(n)
 *            = 2*a(n-1) - 2*a(n-4) + 2*a(n-5) - 2*a(n-6) + a(n-7) (n>6)
 *            Initial terms: a(0)=1, a(1)=3, a(2)=5, a(3)=11, a(4)=19,
 *            a(5)=34, a(6)=62
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   init_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/init_base.h"

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array holding terms a(0)...a(m) */
    static size_t cache_size = 0;    /* allocated size of cache (in elements) */
    static size_t cache_len  = 0;    /* number of initialized entries in cache */

    /* Ensure we can address index n */
    if (cache_len <= (size_t)n) {
        /* Resize if necessary */
        if (cache_size <= (size_t)n) {
            size_t new_size = cache_size ? cache_size * 2 : 16;
            while (new_size <= (size_t)n) new_size <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failed */
            cache = tmp;
            cache_size = new_size;
        }

        /* Initialize base case if necessary */
        if (cache_len == 0) {
            init_base(cache);
            cache_len = 7; /* 0..6 are initialized */
        }

        /* For n <= 6 simply return the cached value */
        if ((idx_t)n<7) return cache[(size_t)n];

        /* Compute missing terms up to n using the recurrence */
        size_t i;
        for (i = cache_len; i <= (size_t)n; ++i) {
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-4];
            seq_t t3 = cache[i-5];
            seq_t t4 = cache[i-6];
            seq_t t5 = cache[i-7];
            /* a(i) = 2*a(i-1) - 2*a(i-4) + 2*a(i-5) - 2*a(i-6) + a(i-7) */
            cache[i] = (t1 << 1) - (t2 << 1) + (t3 << 1) - (t4 << 1) + t5;
        }
        cache_len = n + 1; /* we now have values up to n */
    }

    return cache[(size_t)n];
}
