/*
 * A312928  Coordination sequence Gal.3.16.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) (n > 6) Initial values: a(0) =
 *            1, a(1) = 4, a(2) = 9, a(3) = 13, a(4) = 18, a(5) = 22, a(6) =
 *            26 The sequence is defined for n ≥ 0 with offset 0
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to handle arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static size_t  ready = 0;   /* number of entries already computed */

    /* Ensure enough capacity for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap > 0) ? cap * 2 : 16;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Initialize the known first seven values once */
    if (ready == 0) {
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 9;
        cache[3] = 13;
        cache[4] = 18;
        cache[5] = 22;
        cache[6] = 26;
        ready   = 7;
    }

    /* If the value is already computed, return it immediately */
    if ((size_t)n < ready)
        return cache[n];

    /* Compute missing terms up to n */
    for (size_t i = ready; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 5] - cache[i - 6];
    }
    ready = (size_t)n + 1;
    return cache[n];
}
