/*
 * A393503  Number of paths in the n-necklace graph.
 * https://oeis.org/A393503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393503: Number of paths in the n‑necklace graph.
 *
 * Formula: a(n) = (77*4^n - 128)*n/6
 *
 * Recurrence (valid for n ≥ 5):
 *   a(n) = 10*a(n-1) - 33*a(n-2) + 40*a(n-3) - 16*a(n-4)
 *
 * Initial values (for n = 1 .. 4):
 *   a(1) =   30
 *   a(2) =  368
 *   a(3) = 2400
 *   a(4) = 13056
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.  Index 0 is unused. */
    static seq_t *cache   = NULL;   /* array of size (cap) */
    static size_t cache_size = 0;   /* largest index already stored */
    static size_t capacity   = 0;   /* allocated array size (>= cache_size+1) */

    /* ---- 1. initialise cache on first call -------------------------------- */
    if (cache == NULL) {
        /* We need space for indices 1..max(n,4).  Index 0 is unused. */
        capacity = (size_t)(n >= 4 ? n + 1 : 5);
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failed */

        /* Base terms, valid for all n ≥ 1. */
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)368;
        cache[3] = (seq_t)2400;
        cache[4] = (seq_t)13056;
        cache_size = 4;

        /* If the requested n is already covered, return it. */
        if (n <= cache_size) return cache[(size_t)n];
    }

    /* ---- 2. grow cache if needed ------------------------------------------ */
    if ((size_t)n > cache_size) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < capacity) new_cap = capacity;  /* never shrink */
        if (new_cap > capacity) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;        /* reallocation failed */
            cache = tmp;
            capacity = new_cap;
        }

        /* Compute missing terms up to n using the recurrence. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t term1 = cache[(size_t)(i - 1)] * (seq_t)10;
            term1 += cache[(size_t)(i - 3)] * (seq_t)40;
            seq_t term2 = cache[(size_t)(i - 2)] * (seq_t)33;
            term2 += cache[(size_t)(i - 4)] * (seq_t)16;
            cache[(size_t)i] = term1 - term2;   /* term1 > term2, no underflow */
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)n];
}
