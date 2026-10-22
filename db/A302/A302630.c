/*
 * A302630  Number of nX3 0..1 arrays with every element equal to 0, 1, 3 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = a(n-1) + 9*a(n-3) - 4*a(n-4) + 2*a(n-5) - 10*a(n-6) + 4*a(n-7) + 4*a(n-9)  for n > 13
   Initial values (1‑based indexing):
     a(1)=3, a(2)=9, a(3)=8, a(4)=19, a(5)=42, a(6)=82,
     a(7)=189, a(8)=469, a(9)=1029, a(10)=2372, a(11)=5661,
     a(12)=12788, a(13)=29440                                                 */
#include <stddef.h>
#include <stdlib.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* static cache: 1‑based indexing */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of elements stored (index 0 unused) */
    static size_t cache_cap  = 0; /* current capacity */

    /* initial values, index 0 unused */
    static const seq_t init[14] = {
        0ULL, /* 0 */
        3,    /* 1 */
        9,    /* 2 */
        8,    /* 3 */
        19,   /* 4 */
        42,   /* 5 */
        82,   /* 6 */
        189,  /* 7 */
        469,  /* 8 */
        1029, /* 9 */
        2372, /*10 */
        5661, /*11 */
        12788,/*12 */
        29440 /*13 */
    };

    /* Ensure cache large enough for the requested n (indexing 1‑based) */
    if (n == 0) return 0; /* safety for potential 0 call */
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        /* grow capacity exponentially */
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= idx) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – return 0 (undefined behaviour beyond this point) */
            return 0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Fill missing entries up to n */
    if (idx >= cache_size) {
        for (size_t i = cache_size; i <= idx; ++i) {
            if (i < 14) {
                cache[i] = init[i];
            } else {
                /* apply empirical recurrence */
                cache[i] = cache[i-1]
                          + 9 * cache[i-3]
                          - 4 * cache[i-4]
                          + 2 * cache[i-5]
                          - 10 * cache[i-6]
                          + 4 * cache[i-7]
                          + 4 * cache[i-9];
            }
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
