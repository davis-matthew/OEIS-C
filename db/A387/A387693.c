/*
 * A387693  a(n) = Sum_{k=0..floor(n/4)} 2^(n-4*k) * binomial(2*n-6*k+1,2*k).
 * https://oeis.org/A387693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used for efficient calculation:
   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=19, a(5)=52, a(6)=148, a(7)=416
   a(n)=4*a(n‑1)-4*a(n‑2)+2*a(n‑4)+4*a(n‑5)-a(n‑8)   for n≥8          */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t max_computed = 0;  /* index of last value stored */

    /* Ensure cache large enough for all indices up to n */
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
        cache_size = need > 8 ? need : 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* Initialise the first eight terms (known from OEIS) */
        cache[0] = 1;  cache[1] = 2;  cache[2] = 4;  cache[3] = 8;
        cache[4] = 19; cache[5] = 52; cache[6] = 148; cache[7] = 416;
        max_computed = 7;
    } else if (need > cache_size) {
        cache_size = need;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* Return already computed value */
    if ((size_t)n <= max_computed) {
        return cache[(size_t)n];
    }

    /* Compute missing terms up to n using the recurrence */
    size_t start = max_computed + 1;
    if (start < 8) {          /* we need the recurrence only for n >= 8 */
        start = 8;
    }
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t val = 4 * cache[i - 1];
        val = val - 4 * cache[i - 2];
        val = val + 2 * cache[i - 4];
        val = val + 4 * cache[i - 5];
        val = val - cache[i - 8];
        cache[i] = val;
    }
    max_computed = (size_t)n;
    return cache[(size_t)n];
}
