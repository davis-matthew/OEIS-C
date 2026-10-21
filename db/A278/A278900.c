/*
 * A278900  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 107", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A278900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=1, a(3)=15, a(4)=4, a(5)=55, a(6)=0,
   a(7)=247, a(8)=20, a(9)=983, a(10)=20, a(11)=4087, a(12)=84,
   a(13)=16215, a(14)=84, a(15)=65495.
   For n>15:
       a(n) = 5*a(n-2) - 20*a(n-6) + 16*a(n-8)   (OEIS A278900)   */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_len = 0;   /* highest computed index (>=0) */

    /* initial values for n = 0 .. 15 */
    static const seq_t init[16] = {
        1ULL, 1ULL, 1ULL, 15ULL, 4ULL, 55ULL, 0ULL,
        247ULL, 20ULL, 983ULL, 20ULL, 4087ULL,
        84ULL, 16215ULL, 84ULL, 65495ULL
    };

    /* first call: allocate cache and copy initial values */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;            /* out of memory → zero */
        memcpy(cache, init, cache_cap * sizeof(seq_t));
        cache_len = 15;                         /* last index stored */
    }

    /* if the requested index is within the already
       computed range, return it immediately */
    if (n <= (idx_t)cache_len) return cache[(size_t)n];

    /* extend the cache if necessary */
    size_t needed = (size_t)n;
    if (needed >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= needed) new_cap = new_cap ? new_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;         /* out of memory → zero */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms sequentially up to n */
    for (size_t i = cache_len + 1; i <= needed; ++i) {
        /* sum = 5*a(i-2) + 16*a(i-8) */
        seq_t sum = 5 * cache[i - 2];
        sum += 16 * cache[i - 8];
        /* subtract = 20*a(i-6) */
        seq_t sub = 20 * cache[i - 6];
        cache[i] = sum - sub; /* inequality guarantees non‑negative result */
    }
    cache_len = needed;
    return cache[needed];
}
