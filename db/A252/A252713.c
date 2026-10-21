/*
 * A252713  Number of (n+2) X (2+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 45, u64 n <= 93, u128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + 7*a(n-3) - 7*a(n-4) - 12*a(n-6) + 12*a(n-7) , n > 9
 * Initial values: a(1)=52, a(2)=46, a(3)=56, a(4)=76, a(5)=102,
 *                 a(6)=154, a(7)=220, a(8)=306, a(9)=478
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;        /* Allocated size (indexable up to cache_size-1) */
    static size_t computed_max = 0;      /* Highest n that has been computed */

    /* Ensure the cache can hold index n */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) abort();        /* Allocation failure */
        /* Copy old values */
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        /* Initialise newly allocated cells that belong to the base range (1..9) */
        static const seq_t base_vals[10] = {
            (seq_t)0,
            (seq_t)52, (seq_t)46, (seq_t)56, (seq_t)76, (seq_t)102,
            (seq_t)154, (seq_t)220, (seq_t)306, (seq_t)478
        };
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i <= 9) new_cache[i] = base_vals[i];
            else new_cache[i] = (seq_t)0; /* placeholder, will be computed later */
        }
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute all terms up to n that have not yet been computed */
    if (computed_max < n) {
        static const seq_t base_vals[10] = {
            (seq_t)0,
            (seq_t)52, (seq_t)46, (seq_t)56, (seq_t)76, (seq_t)102,
            (seq_t)154, (seq_t)220, (seq_t)306, (seq_t)478
        };
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = base_vals[i];
            } else {
                /* a(i) = a(i-1) + 7*a(i-3) - 7*a(i-4) - 12*a(i-6) + 12*a(i-7) */
                seq_t pos = cache[i-1] +
                            (seq_t)7 * cache[i-3] +
                            (seq_t)12 * cache[i-7];
                seq_t neg = (seq_t)7 * cache[i-4] +
                            (seq_t)12 * cache[i-6];
                cache[i] = pos - neg;
            }
        }
        computed_max = n;
    }

    return cache[n];
}
