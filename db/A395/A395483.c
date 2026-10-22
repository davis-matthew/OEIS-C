/*
 * A395483  Number of derangements of [n] with 4 descents.
 * https://oeis.org/A395483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A395483: Number of derangements of [n] with 4 descents
   Recurrence (n > 14):
       a(n) = 30*a(n-1) - 400*a(n-2) + 3122*a(n-3) - 15799*a(n-4)
              + 54176*a(n-5) - 127512*a(n-6) + 203456*a(n-7)
              - 210672*a(n-8) + 127872*a(n-9) - 34560*a(n-10)
   Initial values (n = 0..14):
       0, 0, 0, 0, 0, 0, 24, 480, 5984, 57640,
       470504, 3426320, 22979920, 144970904, 872919960 */
#include <stdlib.h>

/* Static cache of computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* maximum index + 1 that is already present */

/* Store the 15 known initial terms */
static const seq_t init_vals[15] = {
    0, 0, 0, 0, 0, 0,
    24, 480, 5984, 57640,
    470504, 3426320, 22979920,
    144970904, 872919960
};

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough to hold indices 0 .. n */
    size_t needed = (size_t)n + 1;
    /* The recurrence is only defined for n ≥ 15, but we must reserve space for the
       first 15 already‑known values regardless of needed. */
    if (cache_cap < (needed > 15 ? needed : 15)) {
        size_t new_cap = (needed > 15 ? needed : 15);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return zero */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Load the 15 initial values only once. */
    if (cache_len < 15) {
        for (size_t i = cache_len; i < 15; ++i)
            cache[i] = init_vals[i];
        cache_len = 15;
    }

    /* For n < 15 we already have the value stored. */
    if ((size_t)n < 15)
        return cache[(size_t)n];

    /* Compute values up to a(n), using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Sum of positive terms (ordering ensures this sum is at least as big
           as the sum of negative terms, so the final subtraction stays
           non‑negative in unsigned arithmetic). */
        seq_t pos = cache[i-1] * 30
                  + cache[i-3] * 3122
                  + cache[i-5] * 54176
                  + cache[i-7] * 203456
                  + cache[i-9] * 127872;

        seq_t neg = cache[i-2] * 400
                  + cache[i-4] * 15799
                  + cache[i-6] * 127512
                  + cache[i-8] * 210672
                  + cache[i-10] * 34560;

        cache[i] = pos - neg;   /* pos ≥ neg for all valid n */
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
