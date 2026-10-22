/*
 * A312822  Coordination sequence Gal.6.577.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A312822 - Coordination sequence Gal.6.577.1
 * Recurrence (for n > 13):
 *   a(n) = 2*a(n-2) - 3*a(n-4) + 4*a(n-6) - 3*a(n-8) + 2*a(n-10) - a(n-12)
 * Initial values:
 *   a(0)=1, a(1)=4, a(2)=8, a(3)=16, a(4)=22, a(5)=28,
 *   a(6)=34, a(7)=34, a(8)=42, a(9)=42, a(10)=52, a(11)=60,
 *   a(12)=66, a(13)=72
 *
 * This function is memoised and works for any order of calls and any
 * n ≥ 0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_values[14] = {
    1, 4, 8, 16, 22, 28, 34, 34, 42, 42, 52, 60, 66, 72
};

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of elements already stored */
    static size_t cache_cap  = 0;  /* allocated capacity */

    /* Ensure cache is initialised and has at least the first 14 terms */
    if (cache == NULL) {
        cache_cap  = 14;
        cache_size = 14;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure, return 0 as fallback */
        for (size_t i = 0; i < 14; ++i) cache[i] = init_values[i];
    }

    /* If the requested index is within pre‑computed range, return it */
    if (n < cache_size) return cache[n];

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* All indices used in the recurrence are guaranteed to exist
           because they are less than i, except for i < 14 which is
           handled above. */
        seq_t sum_pos = 2 * cache[i - 2] + 4 * cache[i - 6] + 2 * cache[i - 10];
        seq_t sum_neg = 3 * cache[i - 4] + 3 * cache[i - 8] + cache[i - 12];
        cache[i] = sum_pos - sum_neg; /* subtraction safe as sum_pos >= sum_neg */
    }

    cache_size = n + 1;
    return cache[n];
}
