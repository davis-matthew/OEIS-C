/*
 * A302266  Number of 2Xn 0..1 arrays with every element equal to 0, 1, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302266:
   a(1)=2, a(2)=8, a(3)=29, a(4)=105
   for n>4: a(n)=3*a(n-1)+a(n-2)+4*a(n-3)+4*a(n-4)        */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   up_to = 0;   /* largest n with cache[n‑1] defined */

    /* first call – initialise with the first four terms */
    if (!cache) {
        cap   = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory guard */

        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)8;   /* a(2) */
        cache[2] = (seq_t)29;  /* a(3) */
        cache[3] = (seq_t)105; /* a(4) */
        up_to = 4;
    }

    /* already computed? */
    if (n <= up_to)
        return cache[n - 1];

    /* grow the cache if needed */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache); cache = NULL; cap = 0; up_to = 0;
            return (seq_t)0;                 /* allocation failed */
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        seq_t term = (seq_t)3 * cache[i - 2]          /* 3*a(i‑1) */
                  +          cache[i - 3]          /*   +a(i‑2) */
                  + (seq_t)4 * cache[i - 4]       /*   +4*a(i‑3) */
                  + (seq_t)4 * cache[i - 5];     /*   +4*a(i‑4) */
        cache[i - 1] = term;
    }
    up_to = n;
    return cache[n - 1];
}
