/*
 * A315447  Coordination sequence Gal.5.112.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 2*a(n-5) - a(n-6) (n > 6)
 *            Initial values: a(0)=1, a(1)=6, a(2)=11, a(3)=16, a(4)=20,
 *            a(5)=24, a(6)=29
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;           /* array of computed a(k) */
    static size_t cap   = 0;              /* current allocated capacity */
    static idx_t upTo   = 0;              /* highest index already computed */
    static int   init_done = 0;           /* flag to run one‑time init */

    /* initial values for n = 0 .. 6  */
    static const seq_t init[7] = {1, 6, 11, 16, 20, 24, 29};

    if (!init_done) {
        /* Allocate a reasonable starting size (at least 8) */
        cap = (n + 1 > 8) ? (n + 1) : 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (int i = 0; i < 7; ++i)           /* set the known values */
            cache[i] = init[i];
        upTo = 6;                              /* we know up to a(6) */
        init_done = 1;
    }

    /* Expand cache if caller requests a larger index */
    if (n + 1 > cap) {
        cap = n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Compute missing terms with the recurrence, up to n */
    if (n > upTo) {
        for (idx_t k = upTo + 1; k <= n; ++k) {
            seq_t a1 = cache[k - 1];
            seq_t a2 = cache[k - 2];
            seq_t a4 = cache[k - 4];
            seq_t a5 = cache[k - 5];
            seq_t a6 = cache[k - 6];
            cache[k] = 2 * a1 - a2 - a4 + 2 * a5 - a6;
        }
        upTo = n;
    }

    return cache[n];
}
