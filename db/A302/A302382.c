/*
 * A302382  Number of 3Xn 0..1 arrays with every element equal to 1, 2, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A302382: Number of 3×n 0..1 arrays with each element adjacent to
 * 1–4 ones horizontally, diagonally or antidiagonally, with upper left
 * element zero.
 *
 * Recurrence (valid for n > 15):
 *   a(n) =  4*a(n-1) + 15*a(n-2) + 14*a(n-3) + 19*a(n-4)
 *           +  8*a(n-5) +  9*a(n-6) – 9*a(n-7) + 4*a(n-8)
 *           – 5*a(n-9) + 3*a(n-10) – 3*a(n-11) – 5*a(n-12)
 *           +   a(n-13) – 2*a(n-14)
 *
 * Initial values (n = 1..15):
 *   a(1)=0,  a(2)=11,  a(3)=76,  a(4)=430,  a(5)=2886,
 *   a(6)=19215, a(7)=127535, a(8)=847604, a(9)=5631313,
 *   a(10)=37416247, a(11)=248600404, a(12)=1651756796,
 *   a(13)=10974620983, a(14)=72917744789, a(15)=484481101148
 */

#include <stdlib.h>

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Initial terms. */
    static const seq_t init[15] = {
        0ULL, 11ULL, 76ULL, 430ULL, 2886ULL,
        19215ULL, 127535ULL, 847604ULL, 5631313ULL,
        37416247ULL, 248600404ULL, 1651756796ULL,
        10974620983ULL, 72917744789ULL, 484481101148ULL
    };
    const size_t init_len = 15U;

    /* Recurrence coefficients. */
    static const int coeffs[14] = {
        4, 15, 14, 19, 8, 9, -9, 4, -5, 3, -3, -5, 1, -2
    };

    /* Static memoisation cache. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (in elements)  */
    static size_t cache_size = 0;  /* number of values actually stored   */

    /* Target size needed (cache must hold all terms up to n). */
    size_t target = (size_t)n;
    size_t needed = target > init_len ? target : init_len;

    /* Allocate or grow the cache if necessary. */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        if (new_cap < needed) new_cap = needed;
        while (new_cap < needed) new_cap <<= 1;   /* double until big enough */

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;                /* allocation failed */

        /* Copy existing computed values. */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        /* Free old storage and update pointers/capacity. */
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Ensure the initial 15 terms are in the cache. */
    if (cache_size < init_len) {
        for (size_t i = cache_size; i < init_len; ++i)
            cache[i] = init[i];
        cache_size = init_len;
    }

    /* Compute any missing terms beyond the current cache_size. */
    if (target > cache_size) {
        for (size_t idx = cache_size; idx < target; ++idx) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            /* idx is 0‑based (a(idx+1)).  Use the 14 previous terms. */
            for (int j = 1; j <= 14; ++j) {
                int c = coeffs[j-1];
                seq_t val = cache[idx - j];
                if (c >= 0)
                    pos_sum += val * (seq_t)c;
                else
                    neg_sum += val * (seq_t)(-c);
            }
            cache[idx] = pos_sum - neg_sum;
        }
        cache_size = target;
    }

    return cache[n-1];
}
