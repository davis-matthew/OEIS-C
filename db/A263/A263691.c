/*
 * A263691  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -2 to 2 places and every three consecutive
 *          elements having its maximum within 3 of its minimum.
 * https://oeis.org/A263691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) for n > 12, with
 *            initial values: a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 14, a(5) =
 *            14, a(6) = 16, a(7) = 22, a(8) = 36, a(9) = 56, a(10) = 85,
 *            a(11) = 125, a(12) = 189
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 52, u64 n <= 106,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[13] = {
    (seq_t)0,   /* index 0 unused */
    (seq_t)1,   /* a(1) */
    (seq_t)2,   /* a(2) */
    (seq_t)6,   /* a(3) */
    (seq_t)14,  /* a(4) */
    (seq_t)14,  /* a(5) */
    (seq_t)16,  /* a(6) */
    (seq_t)22,  /* a(7) */
    (seq_t)36,  /* a(8) */
    (seq_t)56,  /* a(9) */
    (seq_t)85,  /* a(10) */
    (seq_t)125, /* a(11) */
    (seq_t)189  /* a(12) */
};

seq_t a(idx_t n) {
    /* Handle offset: a(1) is first term */
    if (n <= (idx_t)12) {
        return init[n];
    }

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached_to = 0;   /* highest index computed in cache */

    /* Allocate or resize cache as needed */
    if (cache == NULL) {
        size_t cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        cached_to = 0;
    } else if ((size_t)cached_to + 1 < n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* reallocation failure */
        cache = tmp;
    }

    /* Ensure the first 12 values are in the cache */
    if (cached_to < 12) {
        for (idx_t i = 1; i <= 12; ++i) {
            cache[i] = init[i];
        }
        cached_to = 12;
    }

    /* Compute values up to n if necessary */
    while (cached_to < n) {
        /* recurrence: a(i) = a(i-1) + a(i-2) - a(i-3) + a(i-4) */
        seq_t v = cache[cached_to] + cache[cached_to - 1];
        v = v - cache[cached_to - 2];
        v = v + cache[cached_to - 3];
        ++cached_to;
        cache[cached_to] = v;
    }

    return cache[n];
}
