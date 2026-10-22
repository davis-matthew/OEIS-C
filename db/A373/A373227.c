/*
 * A373227  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 7; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 7.
 * https://oeis.org/A373227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 7; a(n) = a(n-a(n-1)) + a(n-a(n-2)) +
 *            a(n-a(n-3)) for n > 7
 * Range:     u8 n <= 354, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* trivial branch for the initial seven terms */
    if (n <= 7)
        return (seq_t)n;

    /* static cache and bookkeeping variables */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  computed_max = 0;

    /* first call: allocate minimal cache, initialise base values */
    if (!cache) {
        cache_cap = (size_t)n + 1;       /* enough to hold requested n */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */

        cache[0] = (seq_t)0;             /* safety for possible 0‑index uses */

        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = (seq_t)i;
        computed_max = 7;
    }

    /* enlarge cache if the requested index is beyond current capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n on demand */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            idx_t i1 = i - (idx_t)cache[i - 1];
            idx_t i2 = i - (idx_t)cache[i - 2];
            idx_t i3 = i - (idx_t)cache[i - 3];
            /* the recurrence guarantees i1,i2,i3 >= 1, otherwise cache[0] protects us */
            cache[i] = cache[i1] + cache[i2] + cache[i3];
        }
        computed_max = n;
    }

    return cache[n];
}
