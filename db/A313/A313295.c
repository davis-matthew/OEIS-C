/*
 * A313295  Coordination sequence Gal.5.90.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 a(2) = 9 a(3) = 15 a(4) = 21 a(5) = 27 a(6) =
 *            33 a(7) = 39 a(8) = 45 a(9) = 50 a(10) = 54 For n > 10:
 *            a(n)=2*a(n-1)-2*a(n-2)+2*a(n-3)-2*a(n-4)+2*a(n-5)
 *            -2*a(n-6)+2*a(n-7)-2*a(n-8)+2*a(n-9)-a(n-10)
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A313295. */
seq_t a(idx_t n)
{
    size_t ui = (size_t)n;          /* safe conversion for indexing   */
    static seq_t *cache = NULL;     /* memoisation buffer             */
    static size_t cap = 0;          /* allocated capacity              */
    static size_t len = 0;          /* number of values stored         */
    static const seq_t init[11] = {
        1, 4, 9, 15, 21, 27, 33, 39, 45, 50, 54
    };

    if (ui < 11) {
        /* return the initial values directly */
        return init[ui];
    }

    /* initialise the cache the first time a(n) for n ≥ 11 is requested */
    if (cache == NULL) {
        size_t start_cap = 16;                  /* arbitrary starting size   */
        cache = (seq_t *)malloc(start_cap * sizeof(seq_t));
        cap   = start_cap;
        /* copy the known initial terms */
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init[i];
        }
        len   = 11;
    }

    /* extend capacity if necessary */
    if (ui >= cap) {
        size_t new_cap = cap;
        while (new_cap <= ui) new_cap <<= 1;    /* double until big enough */
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cap   = new_cap;
    }

    /* compute any missing terms up to ui */
    if (ui >= len) {
        for (size_t i = len; i <= ui; ++i) {
            cache[i] = (seq_t)(
                2 * cache[i-1]
                - 2 * cache[i-2]
                + 2 * cache[i-3]
                - 2 * cache[i-4]
                + 2 * cache[i-5]
                - 2 * cache[i-6]
                + 2 * cache[i-7]
                - 2 * cache[i-8]
                + 2 * cache[i-9]
                - cache[i-10]
            );
        }
        len = ui + 1;
    }

    /* return the requested term */
    return cache[ui];
}
