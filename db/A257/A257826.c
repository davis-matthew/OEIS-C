/*
 * A257826  Positive integers whose square is the sum of 88 consecutive
 *          squares.
 * https://oeis.org/A257826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 394*a(n-4) - a(n-8) (n ≥ 9) Initial terms: a(1)=2222,
 *            a(2)=2530, a(3)=39358, a(4)=55990, a(5)=872938, a(6)=994598,
 *            a(7)=15506810, a(8)=22059818
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* cache[0] unused, cache[1..] are terms */
    static size_t cap = 0;               /* size of allocated cache */
    static size_t last = 0;              /* highest index computed so far */
    static int initialized = 0;

    /* Initialise cache with the first 8 terms */
    if (!initialized) {
        /* allocate space for indices 0..8 */
        size_t need = 9;                 /* 0..8 inclusive */
        cache = (seq_t *)realloc(NULL, need * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed, return 0 as fallback */

        /* set unused 0 element */
        cache[0] = (seq_t)0;
        /* constant initial values */
        cache[1] = 2222ULL;
        cache[2] = 2530ULL;
        cache[3] = 39358ULL;
        cache[4] = 55990ULL;
        cache[5] = 872938ULL;
        cache[6] = 994598ULL;
        cache[7] = 15506810ULL;
        cache[8] = 22059818ULL;
        cap = need;
        last = 8;
        initialized = 1;
    }

    /* Extend cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms sequentially up to n */
    if (n > last) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            /* Recurrence requires i≥9 */
            if (i >= 9) {
                cache[i] = (seq_t)394 * cache[i - 4] - cache[i - 8];
            } else {
                /* for safety, but we already filled 1..8 */
                cache[i] = 0;
            }
        }
        last = (size_t)n;
    }

    return cache[n];
}
