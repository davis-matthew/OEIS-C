/*
 * A279441  Number of ways to place 7 points on an n X n square grid so that
 *          no more than 2 points are on a vertical or horizontal straight
 *          line.
 * https://oeis.org/A279441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 43, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279441:
   Number of ways to place 7 points on an n × n square grid
   so that no more than 2 points are on a vertical or horizontal
   line.

   The values satisfy the 15‑term recurrence
       a(n) = 15*a(n‑1) –105*a(n‑2) +455*a(n‑3) –1365*a(n‑4)
              +3003*a(n‑5) –5005*a(n‑6) +6435*a(n‑7) –6435*a(n‑8)
              +5005*a(n‑9) –3003*a(n‑10) +1365*a(n‑11) –455*a(n‑12)
              +105*a(n‑13) –15*a(n‑14) +a(n‑15)
   for n > 15, with the first 15 terms listed below.

   This implementation keeps a cache of all computed values so any
   call order works, and uses a signed 128‑bit accumulator when the
   build is plain C (where seq_t is an unsigned integer type) and
   uses the seq_t type directly when compiled as C++ (where seq_t
   may be an arbitrary‑precision big integer class).
   The algorithm runs in O(n) time and O(n) space for a given n. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static const seq_t init_15[15] = {
    0ULL, 0ULL, 0ULL,      /* a(1), a(2), a(3) */
    816ULL, 93000ULL, 2602800ULL, 35526120ULL,
    309328320ULL, 1972234656ULL, 9989784000ULL,
    42369069600ULL, 155993500080ULL, 511660972680ULL,
    1524225598896ULL, 4185197289000ULL
};

static const int coeff[15] = {
     15,  -105,   455,  -1365,   3003,  -5005,   6435,  -6435,
    5005,  -3003,  1365,   -455,    105,    -15,       1
};

static seq_t *cache = NULL;      /* array of computed a(n) values (1‑based) */
static idx_t cache_cap = 0;      /* current size of cache */

seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined for n < 1 */

    /* Ensure cache is large enough and filled up to n */
    if (n > cache_cap) {
        /* Grow cache to at least 15 values first */
        if (cache_cap < 15) {
            size_t new_cap = 15;
            if (!cache) {
                cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            } else {
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            }
            for (size_t i = 0; i < 15; ++i) {
                cache[i] = init_15[i];
            }
            cache_cap = 15;
        }

        /* Extend cache to n and compute necessary values */
        if (n > cache_cap) {
            size_t new_cap = (size_t)n;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

            /* Compute values from cache_cap+1 to n */
            for (idx_t i = cache_cap + 1; i <= n; ++i) {
                if (i <= 15) {
                    cache[i-1] = init_15[i-1];
                    continue;
                }

#if defined(__cplusplus)
                /* C++ build: use seq_t operations directly */
                seq_t sum = 0;
                for (int k = 0; k < 15; ++k) {
                    int c = coeff[k];
                    seq_t term = (seq_t)(c >= 0 ? c : -c);
                    term *= cache[i-2-(k-0)];
                    if (c >= 0)
                        sum += term;
                    else
                        sum -= term;
                }
                cache[i-1] = sum;
#else
                /* Plain C build: use signed 128‑bit accumulator */
                __int128 sum = 0;
                for (int k = 0; k < 15; ++k) {
                    int c = coeff[k];
                    __int128 term = (__int128)(c >= 0 ? c : -c) *
                                    (unsigned long long)cache[i-2-(k-0)];
                    if (c >= 0)
                        sum += term;
                    else
                        sum -= term;
                }
                cache[i-1] = (seq_t)sum;
#endif
            }
            cache_cap = (size_t)n;
        }
    }

    return cache[n-1];
}
