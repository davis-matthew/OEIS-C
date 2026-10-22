/*
 * A307202  Coordination sequence for trivalent node of type alpha' in the
 *          first Moore pentagonal tiling.
 * https://oeis.org/A307202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A307202: Coordination sequence for trivalent node of type alpha'.
 * Formula: a(0)=1, a(1)=3, a(2)=9, a(3)=15, a(4)=21, a(5)=24, a(6)=30, a(7)=42;
 * For n ≥ 0:  a(n+4) = a(n) + d[n mod 4] where d = {21,21,21,27}.
 * The recurrence is repeated for all n, yielding a(n) for any n ≥ 0.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to handle arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of elements stored so far */

    /* delta array for the recurrence */
    static const seq_t delta[4] = {21, 21, 21, 27};

    /* initialise cache with the first 8 terms */
    if (cache == NULL) {
        cache_len = 8;
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* memory allocation failed; return 0 as sentinel */
        cache[0] = 1;   cache[1] = 3;
        cache[2] = 9;   cache[3] = 15;
        cache[4] = 21;  cache[5] = 24;
        cache[6] = 30;  cache[7] = 42;
    }

    /* grow cache if needed */
    while (cache_len <= n) {
        /* Ensure enough capacity for the new element */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= cache_len) new_cap = cache_len * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* memory allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute a[cache_len] using the recurrence */
        idx_t idx = cache_len;              /* current n to compute */
        idx_t prev = idx - 4;               /* n-4 */
        cache[cache_len] = cache[prev] + delta[prev % 4];
        cache_len++;
    }

    return cache[n];
}
