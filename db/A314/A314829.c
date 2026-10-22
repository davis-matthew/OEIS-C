/*
 * A314829  Coordination sequence Gal.4.41.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 9, a(3) = 14, a(4) = 18, a(5) = 22,
 *            a(6) = 27; For n > 6: a(n) = 2*a(n-1) - a(n-2) - a(n-4) +
 *            2*a(n-5) - a(n-6)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[7] = {
    (seq_t)1,
    (seq_t)5,
    (seq_t)9,
    (seq_t)14,
    (seq_t)18,
    (seq_t)22,
    (seq_t)27
};

static seq_t *cache = NULL;  /* cached sequence values */
static size_t cap   = 0;     /* current cache capacity */

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Handle the first seven terms directly. */
    if (n < 7)
        return init[n];

    /* Initialise the cache on first use. */
    if (cap == 0) {
        cap = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory – caller can decide what to do */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init[i];
    }

    /* Grow the cache if necessary. */
    size_t nsize = (size_t)n + 1;
    if (nsize > cap) {
        size_t new_cap = nsize;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory */
        cache = tmp;
        for (size_t i = cap; i < new_cap; ++i) {
            /* recurrence uses only earlier values already in cache */
            seq_t val = (seq_t)2 * cache[i - 1]
                        - cache[i - 2]
                        - cache[i - 4]
                        + (seq_t)2 * cache[i - 5]
                        - cache[i - 6];
            cache[i] = val;
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
