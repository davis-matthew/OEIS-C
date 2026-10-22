/*
 * A313694  Coordination sequence Gal.4.63.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-8) for n > 8; a(0)=1, a(1)=5,
 *            a(2)=10, a(3)=15, a(4)=19, a(5)=24, a(6)=29, a(7)=34, a(8)=39
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Initial values for indices 0 … 8 */
static const seq_t init[9] = {
    1, 5, 10, 15, 19, 24, 29, 34, 39
};

/* Cache for computed terms (dynamic, grows as needed) */
static seq_t *cache = NULL;   /* pointer to allocated array */
static size_t cached_size = 0;/* number of already computed entries */
static size_t capacity = 0;   /* allocated capacity (in seq_t elements) */

/* Compute a(n) for any n >= 0 with memoisation */
seq_t a(idx_t n)
{
    /* small indices: just return the constant table */
    if (n < 9) {
        return init[(size_t)n];
    }

    /* first call: allocate initial capacity and copy the first 9 terms */
    if (cache == NULL) {
        capacity = 9;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) {
            return 0; /* allocation failure */
        }
        for (size_t i = 0; i < 9; ++i) {
            cache[i] = init[i];
        }
        cached_size = 9;
    }

    /* ensure we can index up to n */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        if (new_cap == 0) new_cap = 9;
        while ((size_t)n + 1 > new_cap) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            return 0; /* allocation failure */
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    size_t start = cached_size;
    for (size_t i = start; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 7] - cache[i - 8];
    }
    cached_size = (size_t)n + 1; /* we now have terms 0 … n */

    return cache[(size_t)n];
}
